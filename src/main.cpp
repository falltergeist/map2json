/*
 * Copyright (c) 2015 Falltergeist developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

// C++ standard includes
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

// map2json includes
#include "subtypes.h"

// Third party includes

uint32_t readUINT32(std::ifstream& stream)
{
    uint32_t value;
    char * buff = reinterpret_cast<char *>(&value);
    stream.read(buff, 4);
    std::reverse(buff, buff + 4);
    return value;
}

int32_t readINT32(std::ifstream& stream)
{
    return readUINT32(stream);
}

void usage(std::string binaryName)
{
    std::cout << "MAP to JSON converter v0.0.1" << std::endl;
    std::cout << "Usage: " << binaryName << " <MAP filename>" << std::endl;
}

void readObject(std::ifstream& stream)
{
    readUINT32(stream); // unknown 0
    readINT32(stream);  // position
    readUINT32(stream); // unknown 1
    readUINT32(stream); // unknown 2
    readUINT32(stream); // unknown 3
    readUINT32(stream); // unknown 4
    readUINT32(stream); // frame number
    readUINT32(stream); // orientation
    readUINT32(stream); // FID
    readUINT32(stream); // FLAGS
    readUINT32(stream); // elevation
    uint32_t PID = readUINT32(stream);

    uint32_t type = (PID & 0xFF000000) >> 24;
    uint32_t id  = PID & 0x00FFFFFF;

    readUINT32(stream); // unknown 5
    readUINT32(stream); // unknown 6
    readUINT32(stream); // unknown 7
    readUINT32(stream); // unknown 8
    readUINT32(stream); // SID
    readUINT32(stream); // SID
    int32_t inventorySize = readINT32(stream);
    readUINT32(stream); // unknown 9
    readUINT32(stream); // unknown 10
    readUINT32(stream); // unknown 11

    switch (type)
    {
        case 0: // items
        {
            uint32_t subtype = itemSubtype.at(id);
            switch (subtype)
            {
                case 0: // armor
                    break;
                case 1: // container
                    break;
                case 2: // drug
                    break;
                case 3: // weapon
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
                case 4: // ammo
                    readUINT32(stream);
                    break;
                case 5: // misc
                    readUINT32(stream);
                    break;
                case 6: // key
                    readUINT32(stream);
                    break;
            }
            break;
        }
        case 1: // critters:
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            readUINT32(stream);
            break;
        case 2: // scenery
        {
            uint32_t subtype = scenerySubtype.at(id);
            switch (subtype)
            {
                case 0: // door
                    readUINT32(stream);
                    break;
                case 1: // stairs
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
                case 2: // elevator
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
                case 3: // ladder bottom
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
                case 4: // ladder top
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
                case 5: // generic
                    break;
            }
            break;
        }
        case 3:
            break;
        case 4:
            break;
        case 5:
        {
            switch(id)
            {
                case 12:
                    break;
                // Exit Grids
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                    readUINT32(stream);
                    readUINT32(stream);
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
                default:
                    readUINT32(stream);
                    readUINT32(stream);
                    readUINT32(stream);
                    readUINT32(stream);
                    break;
            }
            break;
        }
    }

    if (inventorySize > 0)
    {
        for (unsigned i = 0; i != inventorySize; ++i)
        {
            readUINT32(stream); // ammount
            readObject(stream);
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream in;

    in.open(filename.c_str(), std::ios_base::binary | std::ios_base::in);
    if (!in.is_open())
    {
        std::cout << "Can't open file: " << filename << std::endl;
        return 1;
    }

    // 4 - version
    readUINT32(in);
    // 16 - name
    readUINT32(in);
    readUINT32(in);
    readUINT32(in);
    readUINT32(in);
    // 4 - default position
    readUINT32(in);
    // 4 - default elevation
    readUINT32(in);
    // 4 - default orientation
    readUINT32(in);
    // 4 - local vars
    uint32_t localVars = readUINT32(in);
    // 4 - SID
    readUINT32(in);
    // 4 - flags
    uint32_t flags = readUINT32(in);
    // 4 - elevations
    uint32_t elevations = 0;
    if ((flags & 2) == 0) elevations++;
    if ((flags & 4) == 0) elevations++;
    if ((flags & 8) == 0) elevations++;

    // 4 - unknown
    readUINT32(in);
    // 4 - global vars
    uint32_t globalVars = readUINT32(in);
    // 4 - map id
    readUINT32(in);
    // 4 - time
    readUINT32(in);
    // 4*44 - unknown
    for (unsigned i = 0; i != 44; ++i) readUINT32(in);
    // 4 * NUM-GLOBAL-VARS
    for (unsigned i = 0; i != globalVars; ++i) readUINT32(in);
    // 4 * NUM-LOCAL-VARS
    for (unsigned i = 0; i != localVars; ++i) readUINT32(in);
    // 100*100 * 2 * 2 * elevations  - tiles
    for (unsigned i = 0; i != 100*100*elevations; ++i) readUINT32(in);

    // SCRIPTS SECTION
    for (unsigned int i = 0; i < 5; i++)
    {
        uint32_t count = readUINT32(in);
        if (count > 0)
        {
            short loop = count;
            if (count%16 > 0 ) loop += 16 - count%16;

            unsigned int check = 0;
            for (unsigned short j = 0; j < loop; j++)
            {
                {
                    uint32_t PID = readUINT32(in);

                    readUINT32(in); // unknown1

                    switch ((PID & 0xFF000000) >> 24)
                    {
                        case 1:
                            readUINT32(in); //unknown 2
                            readUINT32(in); //unknown 3
                            break;
                        case 2:
                            readUINT32(in); //unknown 2
                            break;
                        default:
                            break;
                    }
                    readUINT32(in); //unknown 4
                    readUINT32(in); // scriptId
                    readUINT32(in); //unknown 5
                    readUINT32(in); //unknown 6
                    readUINT32(in); //unknown 7
                    readUINT32(in); //unknown 8
                    readUINT32(in); //unknown 9
                    readUINT32(in); //unknown 10
                    readUINT32(in); //unknown 11
                    readUINT32(in); //unknown 12
                    readUINT32(in); //unknown 13
                    readUINT32(in); //unknown 14
                    readUINT32(in); //unknown 15
                    readUINT32(in); //unknown 16

                    if (j < count)
                    {
                        //std::cout << "Map script: 0x" << std::hex << script->PID() << std::endl;
                    }

                }
                if ((j % 16) == 15)
                {
                    uint32_t v = readUINT32(in);
                    check += v;

                    readUINT32(in); // skip 4
                }
            }
            if (check != count)
            {
                std::cout << "Check error!" << std::endl;
                return 1;
            }
        }
    }

    // OBJECTS section
    uint32_t objectsTotal = readUINT32(in);

    //std::cout << "Objects: " << objectsTotal << std::endl;

    for (unsigned int i = 0; i != elevations; ++i)
        {
            uint32_t objectsOnElevation = readUINT32(in);
            for (unsigned int j = 0; j != objectsOnElevation; ++j)
            {
                readObject(in);
            }
        }


    in.close();
    return 0;
}
