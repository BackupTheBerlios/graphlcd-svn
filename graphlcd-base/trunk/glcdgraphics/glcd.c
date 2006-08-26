/*
 * GraphLCD graphics library
 *
 * glcd.c  -  GLCD file loading and saving
 *
 * based on graphlcd plugin 0.1.1 for the Video Disc Recorder
 *  (c) 2001-2004 Carsten Siebholz <c.siebholz AT t-online.de>
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#include <stdio.h>
#include <syslog.h>

#include <string>

#include "bitmap.h"
#include "glcd.h"
#include "image.h"


namespace GLCD
{

using namespace std;

const char * kGLCDFileSign = "GLC";

/*
#pragma pack(1)
struct tGLCDHeader
{
    char sign[3];    // = "GLC"
    char format;     // D - single image, A - animation
    uint16_t width;  // width in pixels
    uint16_t height; // height in pixels
    // only for animations
    uint16_t count;  // number of pictures
    uint32_t delay;  // delay in ms
};
#pragma pack()
*/

cGLCDFile::cGLCDFile()
{
}

cGLCDFile::~cGLCDFile()
{
}

bool cGLCDFile::Load(cImage & image, const string & fileName)
{
    bool ret = false;
    FILE * fp;
    long fileSize;
    char sign[4];
    uint8_t buf[6];
    uint16_t width;
    uint16_t height;
    uint16_t count;
    uint32_t delay;

    fp = fopen(fileName.c_str(), "rb");
    if (fp)
    {
        // get len of file
        if (fseek(fp, 0, SEEK_END) != 0)
        {
            fclose(fp);
            return false;
        }
        fileSize = ftell(fp);

        // rewind and get Header
        if (fseek(fp, 0, SEEK_SET) != 0)
        {
            fclose(fp);
            return false;
        }

        // read header sign
        if (fread(sign, 4, 1, fp) != 1)
        {
            fclose(fp);
            return false;
        }

        // check header sign
        if (strncmp(sign, kGLCDFileSign, 3) != 0)
        {
            syslog(LOG_ERR, "glcdgraphics: load %s failed, wrong header (cGLCDFile::Load).", fileName.c_str());
            fclose(fp);
            return false;
        }

        // read width and height
        if (fread(buf, 4, 1, fp) != 1)
        {
            fclose(fp);
            return false;
        }

        width = (buf[1] << 8) | buf[0];
        height = (buf[3] << 8) | buf[2];
        if (width == 0 || height == 0)
        {
            syslog(LOG_ERR, "glcdgraphics: load %s failed, wrong header (cGLCDFile::Load).", fileName.c_str());
            fclose(fp);
            return false;
        }

        if (sign[3] == 'D')
        {
            count = 1;
            delay = 10;
            // check file length
            if (fileSize != (long) (height * ((width + 7) / 8) + 8))
            {
                syslog(LOG_ERR, "glcdgraphics: load %s failed, wrong size (cGLCDFile::Load).", fileName.c_str());
                fclose(fp);
                return false;
            }
        }
        else if (sign[3] == 'A')
        {
            // read count and delay
            if (fread(buf, 6, 1, fp) != 1)
            {
                syslog(LOG_ERR, "glcdgraphics: load %s failed, wrong header (cGLCDFile::Load).", fileName.c_str());
                fclose(fp);
                return false;
            }
            count = (buf[1] << 8) | buf[0];
            delay = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
            // check file length
            if (count == 0 ||
                fileSize != (long) (count * (height * ((width + 7) / 8)) + 14))
            {
                syslog(LOG_ERR, "glcdgraphics: load %s failed, wrong size (cGLCDFile::Load).", fileName.c_str());
                fclose(fp);
                return false;
            }
            // Set minimal limit for next image
            if (delay < 10)
                delay = 10;
        }
        else
        {
            syslog(LOG_ERR, "glcdgraphics: load %s failed, wrong header (cGLCDFile::Load).", fileName.c_str());
            fclose(fp);
            return false;
        }

        image.Clear();
        image.width = width;
        image.height = height;
        image.delay = delay;
        unsigned char * bmpdata = new unsigned char[height * ((width + 7) / 8)];
        if (bmpdata)
        {
            for (unsigned int n = 0; n < count; n++)
            {
                if (fread(bmpdata, height * ((width + 7) / 8), 1, fp) != 1)
                {
                    delete[] bmpdata;
                    fclose(fp);
                    image.Clear();
                    return false;
                }
                image.bitmaps.push_back(new cBitmap(width, height, bmpdata));
                ret = true;
            }
            delete[] bmpdata;
        }
        else
        {
            syslog(LOG_ERR, "glcdgraphics: malloc failed (cGLCDFile::Load).");
        }
        fclose(fp);
    }
    if (ret)
        syslog(LOG_DEBUG, "glcdgraphics: image %s loaded.", fileName.c_str());
    return ret;
}

bool cGLCDFile::Save(cImage & image, const string & fileName)
{
    bool ret = false;

    return ret;
}

} // end of namespace
