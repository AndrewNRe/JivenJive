/*
NOTE: IF YOU WANT TO USE TEXT / DEBUG, YOU SHOULD USE THE "meiryo.ttc" truetype collection font file as I use ひらがな for in game debugging.
I can't say any other truetype fonts will work with this loader. But I do know the meiryo.ttc does work (you can find it somewhere in the jungle that is your Windows install).
I refuse to fix this at this time, as it's only debugging functionality that works fine for my machine. And isn't in the shipped product.
*/
struct compositeglyphdata
{
    bit32 unicode_codepoint = 0;
    bit32 renderflags = 0;
    bit16 xTranslate = 0;
    bit16 yTranslate = 0;
    mat2x2 transform = {1.0f, 0, 0, 1.0f};
};

struct printfdebug_data
{
    bit8* prntfFontData = nullptr; 
    bit32* prntfRenderList = nullptr;
    s16 linegap = 0; s16 lowest_descender = 0; s16 highest_ascender = 0; bit16 padding;
    GLuint prntfProgram = 0;
    GLuint prntfSSBO = 0;
    bit32 debugrenderlistindex = 0;
    bit32 fontdatasize = 0;
    bit32 debugnumberofstoredglyphs = 0;
    bit32 debugstartofcompositeglyphs = 0;
};

printfdebug_data debug_printf; //NOTE global debug memory!

#define debugmemoryalloc 67108864
#define prntfsizeofvertexdata  8192

//#define _ADDRESSOF (v) ( &(v) )
//#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define _crt_va_start(ap, v)  (ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v))
#define _crt_va_arg(ap, t) ( *(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define _crt_va_end(ap) (ap = (va_list)0)

bit32 integertocodepoint(bit32 x)
{
    switch(x)
    {
        case 0: return 0x0030;
        case 1: return 0x0031;
        case 2: return 0x0032;
        case 3: return 0x0033;
        case 4: return 0x0034;
        case 5: return 0x0035;
        case 6: return 0x0036;
        case 7: return 0x0037;
        case 8: return 0x0038;
        case 9: return 0x0039;
    }
    return 0;
}


struct TRUETYPECOLLECTION_FontInformation
{
    bit32 a = 0;
};

struct tempGlyphInformation
{
    bit16 Unicode_CodePoint;
    bit32 Unicode_GlyphIndex_InFile = 0;
    s16 numberofcontours = 0;
    bit16 numberofflags = 0;
    vec2* Vertex_Data = nullptr;
    bit32* Controlpointflags = nullptr;
    bit16* Contour_Data = nullptr;
    bit16 numberofcompositeglyphs = 0;
    compositeglyphdata* compglyph;
    bit16 advanceWidth = 0;
    s16 LeftSideBearing = 0;
    s16 BBxMin = 0;
    s16 BByMin = 0;
    s16 BBxMax = 0;
    s16 BByMax = 0;
};

//NOTE: TTFC's byte composition is BIG ENDIAN! So conversion to little endian is required!
bit32 loadttfcVersion2(const char* filepath, void* file_io_memory, void* memory_to_write_to)
{
    bit32 fileloadbytesread = platform_fileload(filepath, file_io_memory);
    
    {//Check version procedure()
        bit16 majorversion = ((bit16*)file_io_memory)[2]; bit16 minorversion = ((bit16*)file_io_memory)[3];  
        EndianSwap16(&majorversion);
        if( majorversion != 2 ) { return 0; } //NOTE: wasn't the truetype collection that i'm trying to support.
    }
    
    bit32 numberoffonts = ((bit32*)file_io_memory)[2]; EndianSwap32(&numberoffonts);
    //Setup post font load data to process the loaded data.
    bit32 file_io_memory_allocation_offsetter = 0;
    TRUETYPECOLLECTION_FontInformation* fontinformationstruct = nullptr;
    bit32* offsetTable = nullptr;
    
    {//SetupTRUETYPECOLLECTION_FontInformation struct(s) alloc()
        fontinformationstruct = (TRUETYPECOLLECTION_FontInformation*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
        file_io_memory_allocation_offsetter += numberoffonts * sizeof(TRUETYPECOLLECTION_FontInformation);
    }
    {//SetupOffsetTable
        offsetTable = (bit32*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
        for(bit32 i = 0; i < numberoffonts; i++, file_io_memory_allocation_offsetter+=sizeof(bit32)) 
        { 
            bit32 flipendian = ((bit32*)file_io_memory)[3+i]; EndianSwap32(&flipendian);
            offsetTable[i] = flipendian;
        }
    }
    
    s16 linegap = 0; s16 lowest_descender = 0; s16 highest_ascender = 0;
    tempGlyphInformation* TempGlyphStorage = nullptr;
    bit16 numberofDesiredGlyphs = 0;
    {//SetupTempGlyphStorage
        bit16 UnicodeCodepointOffset = 0x0020;
        TempGlyphStorage = (tempGlyphInformation*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
        //Basic Latin Codepoints literally ascii really. Except for some interesting Unicode codepoint decisions (like forward slash being the yen symbol...).
        for(; UnicodeCodepointOffset < 0x7F; numberofDesiredGlyphs++, UnicodeCodepointOffset++, file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation)) 
        {
            TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint = UnicodeCodepointOffset;
        }
        TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint  = 0x00A5; numberofDesiredGlyphs++; file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation);
        TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint  = 0x3002; numberofDesiredGlyphs++; file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation);
        TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint  = 0x300C; numberofDesiredGlyphs++; file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation);
        TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint  = 0x300D; numberofDesiredGlyphs++; file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation);
        //The above maps \, 。, and 「」
        
        UnicodeCodepointOffset = 0x3041;
        //Load the ひらがな!
        for(;UnicodeCodepointOffset < 0x309D; numberofDesiredGlyphs++, UnicodeCodepointOffset++, file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation))
        {
            TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint = UnicodeCodepointOffset;
        }
        
        UnicodeCodepointOffset =0x30A1; 
        //Load the カタカナ!
        for(; UnicodeCodepointOffset < 0x30FD; numberofDesiredGlyphs++, UnicodeCodepointOffset++, file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation))
        {
            TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint = UnicodeCodepointOffset;
        }
        
        UnicodeCodepointOffset = 0x4E00;
        //CJK glyphs, there's ~2200 here. And they all appear to be the å¸¸ç”¨æ¼¢å­— which should be all the æ¼¢å­— I need.
        for(; UnicodeCodepointOffset < 0x9FB0; numberofDesiredGlyphs++, UnicodeCodepointOffset++, file_io_memory_allocation_offsetter += sizeof(tempGlyphInformation)) 
        {
            TempGlyphStorage[numberofDesiredGlyphs].Unicode_CodePoint = UnicodeCodepointOffset;
        }
    }
    //If needed: Signature data information.
    //bit32 DSIGtablelen = ((bit32*)file_io_memory)[3+fileloadbytesread+1]; bit32 DSIGtableLocation = ((bit32*)file_io_memory)[3+fileloadbytesread+2];
    
    for(bit32 f = 0; f < 1; f++) //NOTE: with meiryo, i'm only loading the non bold / italics / weirdo font. So only  going once. So if wanting more robust load. Fix it.
    { //And by fix it, I mean, make sure to load bold / italic font prints separately into other spots in memory.
        bit32 fontdatatracker = offsetTable[f];
        bit32 version = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&version);
        if( version == 0x00010000 )
        {
            bit16 font_numTables = ((bit16*)file_io_memory)[fontdatatracker/sizeof(bit16)]; fontdatatracker+= sizeof(bit16); EndianSwap16(&font_numTables);
            bit16 font_searchRange = ((bit16*)file_io_memory)[fontdatatracker/sizeof(bit16)]; fontdatatracker += sizeof(bit16); EndianSwap16(&font_searchRange);
            bit16 font_entrySelector = ((bit16*)file_io_memory)[fontdatatracker/sizeof(bit16)]; fontdatatracker += sizeof(bit16); EndianSwap16(&font_entrySelector);
            bit16 font_rangeShift = ((bit16*)file_io_memory)[fontdatatracker/sizeof(bit16)]; fontdatatracker += sizeof(bit16); EndianSwap16(&font_rangeShift);
            
            //NOTE: data that other tables require to properly parse their data.
            bit16 indexToLocFormat = 0; bit16 numberOfHMetrics = 0; bit16 numGlyphs = 0; bit32* glyphoffsets = 0; bit32 glyphoffsetallocvalue = 0; 
            bit16 unitsPerEm = 0;
            
            bit32 requiredtablesfilled = 0;
            for(;requiredtablesfilled <= 9-1;)
            {
                
                bit16 table = 0; //0x01FF; 
                {//SearchForRelevantTable()
                    bit8 stringbuffer[4];
                    bit8* relevanttabletags[9] { (bit8*)"cmap",  (bit8*)"maxp", (bit8*)"head", (bit8*)"loca", (bit8*)"glyf", (bit8*)"hhea", (bit8*)"hmtx", (bit8*)"kern",  (bit8*)"GPOS" };
                    for(bit8 i = 0; i < 4; i++, fontdatatracker++)
                    {
                        stringbuffer[i] = ((bit8*)file_io_memory)[fontdatatracker];
                        
                        if ( stringbuffer[i] == relevanttabletags[requiredtablesfilled][i])
                        {
                            table |= (1 << requiredtablesfilled);
                        }
                        else
                        {
                            table &= 0x0000;
                        }
                        
                        if(table == 0){ break; }
                    }
                }
                
                switch( (table&0x01FF) )
                {
                    case 0x01: //cmap (MSDN: https://docs.microsoft.com/en-us/typography/opentype/spec/cmap)
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        bit32 startoftable = offset;
                        bit16 version =  ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&version);
                        bit16 numTables =  ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&numTables);
                        
                        for( bit8 i = 0; i < numTables; i++ )
                        {
                            bit16 ERplatformID = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&ERplatformID);
                            bit16 ERencodingID = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&ERencodingID);
                            bit32 ERoffset = ((bit32*)file_io_memory)[offset/sizeof(bit32)]; offset += sizeof(bit32); EndianSwap32(&ERoffset);
                            bool32 isUnicode = false;
                            
                            switch(ERplatformID)
                            {
                                case 0: { isUnicode = true; break; } //Universal Unicode 
                                case 3: { if( ERencodingID == 1 || ERencodingID == 10 ) { isUnicode = true; } break; }  //Windows
                            }
                            
                            if( isUnicode > 0 )
                            {
                                //Then process all the Unicode glyph index values.
                                //file_io_memory[fileloadbytesread + file_io_memory_allocation_offsetter];
                                bit16 format = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&format);
                                switch( format )
                                {
                                    case 4: //Segment mapping to delta values
                                    {
                                        bit16 length = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&length);
                                        bit16 language = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&language);
                                        bit16 segCountX2 = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&segCountX2);
                                        bit16 segCount = segCountX2>>1;
                                        bit16 searchRange = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&searchRange);
                                        searchRange>>=1;
                                        bit16 entrySelector = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&entrySelector);
                                        bit16 rangeShift = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&rangeShift); 
                                        rangeShift>>=1;
                                        
                                        bit16* endCode = (bit16*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                                        file_io_memory_allocation_offsetter += sizeof(bit16) * segCount;
                                        for(bit16 i = 0; i < segCount; i++)
                                        {
                                            endCode[i] = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&endCode[i]);
                                        }
                                        bit16 reservePad =  ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&reservePad);
                                        bit16* startCode = (bit16*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                                        file_io_memory_allocation_offsetter += sizeof(bit16) * segCount;
                                        for(bit16 i = 0; i < segCount; i++)
                                        {
                                            startCode[i] = ((bit16*)file_io_memory)[(startoftable + ERoffset)/sizeof(bit16)]; ERoffset += sizeof(bit16); EndianSwap16(&startCode[i]);
                                        }
                                        s16* idDelta = (s16*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                                        file_io_memory_allocation_offsetter += sizeof(s16) * segCount;
                                        for(bit16 i = 0; i < segCount; i++)
                                        {
                                            idDelta[i] = ((s16*)file_io_memory)[(startoftable + ERoffset)/sizeof(s16)]; ERoffset += sizeof(s16);  EndianSwap16(&idDelta[i]);
                                        }
                                        bit16* idRangeOffset = (bit16*)&((bit8*)file_io_memory)[startoftable+ERoffset];
                                        for(bit16 i = 0; i < segCount; i++, ERoffset += sizeof(bit16))
                                        {
                                            EndianSwap16(&idRangeOffset[i]);
                                        }
                                        for(bit16 g = 0; g < numberofDesiredGlyphs; g++ )
                                        {
                                            if(TempGlyphStorage[g].Unicode_GlyphIndex_InFile == 0)
                                            {
                                                for(bit16 s = 0; s < segCount; s++)
                                                {
                                                    if ( endCode[s] >= TempGlyphStorage[g].Unicode_CodePoint && startCode[s] <= TempGlyphStorage[g].Unicode_CodePoint ) 
                                                    {
                                                        if( idRangeOffset[s] == 0  )
                                                        {
                                                            TempGlyphStorage[g].Unicode_GlyphIndex_InFile = (TempGlyphStorage[g].Unicode_CodePoint + idDelta[s]) % 65536;
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            //TempGlyphStorage[g].Unicode_GlyphIndex_InFile = *(idRangeOffset[s]/2 + ( TempGlyphStorage[g].Unicode_CodePoint - startCode[s] ) + &((bit8*)file_io_memory)[startoftable + ERoffset]);
                                                            TempGlyphStorage[g].Unicode_GlyphIndex_InFile = *(((idRangeOffset[s]/2) + (TempGlyphStorage[g].Unicode_CodePoint - startCode[s]) ) + &idRangeOffset[s]);
                                                            EndianSwap16(&TempGlyphStorage[g].Unicode_GlyphIndex_InFile);
                                                            if(TempGlyphStorage[g].Unicode_GlyphIndex_InFile != 0)
                                                            {
                                                                TempGlyphStorage[g].Unicode_GlyphIndex_InFile = (TempGlyphStorage[g].Unicode_GlyphIndex_InFile + idDelta[s]) % 65536; //In practically all cases, this is just adding 0.
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        
                                        file_io_memory_allocation_offsetter -= sizeof(bit16) * segCount;
                                        file_io_memory_allocation_offsetter -= sizeof(bit16) * segCount;
                                        file_io_memory_allocation_offsetter -= sizeof(s16) * segCount;
                                        goto EndOfCmap; //NOTE: IF YOU WANT TO SUPPORT OTHER FONTS BESIDES MEIRYO, YOU MUST (probably) NOT GOTO HERE!
                                        //And support the other format cases besides 4! (Meiryo supports 4,12, and 14 btw).
                                        break;
                                    }
                                }
                            }
                        }
                        EndOfCmap:
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x02: //maxp (MSDN: https://docs.microsoft.com/en-us/typography/opentype/spec/maxp)
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        bit32 version = ((bit32*)file_io_memory)[offset/sizeof(bit32)]; offset += sizeof(bit32); EndianSwap32(&version);
                        numGlyphs = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&numGlyphs);
                        
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x04: //head (MSDN: https://docs.microsoft.com/en-us/typography/opentype/spec/head)
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        bit16 majorVersion = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&majorVersion);
                        bit16 minorVersion = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&minorVersion);
                        //NOTE: fontRevision is "defined by the vendor" as it's data type too it would appear. It says "fixed" on the docs. But I dunno, this works though and is how めいりょ is defined so yea.
                        bit32 fontRevision = ((bit32*)file_io_memory)[offset/sizeof(bit32)]; offset += sizeof(bit32); EndianSwap32(&fontRevision);
                        bit32 checkSumAdjustment = ((bit32*)file_io_memory)[offset/sizeof(bit32)]; offset += sizeof(bit32); EndianSwap32(&checkSumAdjustment);
                        bit32 magicNumber = ((bit32*)file_io_memory)[offset/sizeof(bit32)]; offset += sizeof(bit32); EndianSwap32(&magicNumber);
                        bit16 flags = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&flags);
                        unitsPerEm = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&unitsPerEm);
                        bit64 created = ((bit64*)file_io_memory)[offset/sizeof(bit64)]; offset += sizeof(bit64); EndianSwap64(&created);
                        bit64 modified = ((bit64*)file_io_memory)[offset/sizeof(bit64)]; offset += sizeof(bit64); EndianSwap64(&modified);
                        
                        //NOTE: super big important data from here!
                        s16 xMin = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&xMin);
                        s16 yMin = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&yMin);
                        s16 xMax = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&xMax);
                        s16 yMax = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&yMax);
                        //End the super importance, except maybe directionhint?
                        bit16 macStyle = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&macStyle);
                        bit16 lowestRecPPEM = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&lowestRecPPEM);
                        bit16 fontDirectionHint = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&fontDirectionHint);
                        indexToLocFormat = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&indexToLocFormat);
                        bit16 glyphDataFormat = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&glyphDataFormat);
                        
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x08: //loca (MSDN: https://docs.microsoft.com/en-us/typography/opentype/spec/loca)
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        if(indexToLocFormat == 1)
                        {
                            glyphoffsets  = (bit32*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter]; 
                            glyphoffsetallocvalue = sizeof(bit32) * numGlyphs;
                            file_io_memory_allocation_offsetter +=  glyphoffsetallocvalue;
                            bit32* offsetdata = (bit32*)&((bit8*)file_io_memory)[offset];
                            
                            for(bit16 i = 0; i < numGlyphs; i++)
                            {
                                glyphoffsets[i] = offsetdata[i]; EndianSwap32(&glyphoffsets[i]);
                            }
                        }
                        
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x10: //glyf
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 baseoffset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&baseoffset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        //for(bit16 test = 0; test < numberofDesiredGlyphs; test++)
                        //{
                        //printf("Glyph Unicodepoint: %i, glyphindex: %i\n", TempGlyphStorage[test].Unicode_CodePoint, TempGlyphStorage[test].Unicode_GlyphIndex_InFile);
                        //}
                        
                        for(bit16 g = 0; g < numberofDesiredGlyphs; g++)
                        {
                            
                            if ( TempGlyphStorage[g].Unicode_GlyphIndex_InFile != 0 )
                            {
                                bit32 offset = glyphoffsets[TempGlyphStorage[g].Unicode_GlyphIndex_InFile]; //NOTE: The glyphoffsets were already endian swapped in loca!
                                offset += baseoffset;
                                s16 numberOfContours = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&numberOfContours);
                                TempGlyphStorage[g].BBxMin = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&TempGlyphStorage[g].BBxMin);
                                TempGlyphStorage[g].BByMin = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&TempGlyphStorage[g].BByMin);
                                TempGlyphStorage[g].BBxMax = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&TempGlyphStorage[g].BBxMax);
                                TempGlyphStorage[g].BByMax = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&TempGlyphStorage[g].BByMax);
                                if( numberOfContours >= 0 ) //Simple glyph
                                {
                                    bit16 MaxVertexIterations = 0;
                                    {//GetDataSizes()
                                        TempGlyphStorage[g].Contour_Data = (bit16*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                                        file_io_memory_allocation_offsetter += sizeof(bit16) * numberOfContours;
                                        bit16 i = 0;
                                        for(i; i < numberOfContours; i++, offset += sizeof(bit16))
                                        {
                                            TempGlyphStorage[g].Contour_Data[i] = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; EndianSwap16(&TempGlyphStorage[g].Contour_Data[i]); 
                                            TempGlyphStorage[g].Contour_Data[i]++;
                                            //MaxVertexIterations += TempGlyphStorage[g].Contour_Data[i] - MaxVertexIterations;
                                        }
                                        MaxVertexIterations = TempGlyphStorage[g].Contour_Data[i-1]; 
                                        TempGlyphStorage[g].Vertex_Data = (vec2*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                                        file_io_memory_allocation_offsetter += sizeof(vec2) * MaxVertexIterations;
                                        TempGlyphStorage[g].Controlpointflags = (bit32*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                                        file_io_memory_allocation_offsetter += sizeof(bit32) * ((MaxVertexIterations/32) + 1);
                                    }
                                    
                                    {//DoInstructionProcedure()
                                        bit16 InstructionLength =  ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&InstructionLength);
                                        offset += InstructionLength; //NOTE: if you need the byte instructions for phantom points, fetch them from a for loop instead of indexing over them!
                                    }
                                    
                                    bit8* flagbuffer = &((bit8*)file_io_memory)[offset];
                                    bit16 yoffset = 0;
                                    {//CheckbitflagsproceduretogetYoffset()
                                        bit16 bitflagiterator = 0;
                                        for(bit16 i = 0; i < MaxVertexIterations; bitflagiterator++, i++, offset++)
                                        {
                                            bit8 BitFlag = flagbuffer[bitflagiterator];
                                            bit8 xByteSize = 0;
                                            if( BitFlag & 0x02 ){ 
                                                xByteSize++; }
                                            else { if( (BitFlag & 0x10) == 0 ){ 
                                                    xByteSize += sizeof(bit16); } }
                                            yoffset += xByteSize;
                                            
                                            if( BitFlag & 0x08 ) //Repeat bit.
                                            {
                                                bitflagiterator++; offset++; bit8 numberofdupes = flagbuffer[bitflagiterator];  
                                                for(bit8 d = 0; d < numberofdupes; d++, i++) { yoffset += xByteSize; }
                                            } 
                                        }
                                    }
                                    
                                    {//GetCoordinateDataProcedure()
                                        void* fixedcoordinatedata = &((bit8*)file_io_memory)[offset];
                                        //s16 lastX = 0; s16 lastY = 0; 
                                        s16 X = 0; s16 Y = 0; 
                                        bit16 xoffset = 0;
                                        vec2 compositevector; compositevector.x = 0; compositevector.y = 0;
                                        bit8 BitFlag = 0; bit8 NumberOfRepeatBits = 0; bit16 bitflagiterator = 0;
                                        bit16 flagnumber = 0; bit16 numberofVertexDataWrote = 0;
                                        for(bit16 i = 0; i < MaxVertexIterations; i++)
                                        {
                                            //Check if this bitflag has the repeat bit set.
                                            if(NumberOfRepeatBits > 0) {
                                                NumberOfRepeatBits--; }
                                            else
                                            {
                                                BitFlag = flagbuffer[bitflagiterator];  bitflagiterator++;
                                                if( BitFlag & 0x08 ) {
                                                    NumberOfRepeatBits = flagbuffer[bitflagiterator]; bitflagiterator++; }
                                            }
                                            
                                            //s16 currentX = 0; s16 currentY = 0;
                                            
                                            //Get X coordinate
                                            if(BitFlag & 0x02)
                                            {
                                                s16 xvalue = (s16)((bit8*)fixedcoordinatedata)[xoffset]; xoffset++; 
                                                X += (BitFlag & 0x10) ? xvalue : -xvalue; 
                                            }
                                            else if( !(BitFlag & 0x10) )
                                            {
                                                s16* xptr = (s16*)&((bit8*)fixedcoordinatedata)[xoffset]; xoffset += sizeof(s16); s16 xvalue = (*xptr);EndianSwap16(&xvalue);
                                                X += xvalue;
                                            }
                                            //Get Y coordinate
                                            if(BitFlag & 0x04)
                                            {
                                                s16 yvalue = (s16)((bit8*)fixedcoordinatedata)[yoffset]; yoffset++;
                                                Y += (BitFlag & 0x20) ? yvalue : -yvalue;
                                            }
                                            else if( !(BitFlag & 0x20) )
                                            {
                                                s16* yptr = (s16*)&((bit8*)fixedcoordinatedata)[yoffset]; yoffset+=sizeof(s16); s16 yvalue = (*yptr); EndianSwap16(&yvalue);
                                                Y += yvalue;
                                            }
                                            
                                            TempGlyphStorage[g].Controlpointflags[numberofVertexDataWrote/32] |= ((BitFlag & 0x01) << flagnumber);
                                            
                                            //lastX = currentX; lastY = currentY;
                                            TempGlyphStorage[g].Vertex_Data[numberofVertexDataWrote].x = X;
                                            TempGlyphStorage[g].Vertex_Data[numberofVertexDataWrote].y = Y;
                                            numberofVertexDataWrote++; 
                                            flagnumber++; if(flagnumber == 32) {flagnumber = 0;}
                                        }
                                        TempGlyphStorage[g].numberofcontours = numberOfContours;
                                        if(numberofVertexDataWrote % 32 == 0) {TempGlyphStorage[g].numberofflags = (numberofVertexDataWrote/32);}
                                        else { TempGlyphStorage[g].numberofflags = (numberofVertexDataWrote/32) + 1; }
                                    }
                                    
                                }
                                else
                                {
                                    TempGlyphStorage[g].compglyph = (compositeglyphdata*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter]; 
                                    bit16 ci = 0;
                                    
                                    { CompositeGlyphProcedure:
                                        TempGlyphStorage[g].numberofcompositeglyphs++; file_io_memory_allocation_offsetter += sizeof(compositeglyphdata);
                                        bit16 flag = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&flag);
                                        bit16 glyphIndex = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&glyphIndex);
                                        for(bit32 i = 0; i < numberofDesiredGlyphs; i++)
                                        {
                                            if(glyphIndex == TempGlyphStorage[i].Unicode_GlyphIndex_InFile)
                                            {
                                                TempGlyphStorage[g].compglyph[ci].unicode_codepoint = TempGlyphStorage[i].Unicode_CodePoint;
                                            }
                                        }
                                        
                                        {//GetTranslationSizeProcedure()
                                            if(flag & 0x0001)
                                            {
                                                TempGlyphStorage[g].compglyph[ci].xTranslate = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&TempGlyphStorage[g].compglyph[ci].xTranslate); 
                                                TempGlyphStorage[g].compglyph[ci].yTranslate = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&TempGlyphStorage[g].compglyph[ci].yTranslate);
                                                if(flag & 0x0002) {TempGlyphStorage[g].compglyph[ci].renderflags |= 0x0001;}
                                            }
                                            else
                                            {
                                                bit8 x = ((bit8*)file_io_memory)[offset]; offset++;
                                                bit8 y = ((bit8*)file_io_memory)[offset]; offset++;
                                                if(flag & 0x0002 && x > 0x7F) 
                                                {
                                                    TempGlyphStorage[g].compglyph[ci].xTranslate = (~x); 
                                                    TempGlyphStorage[g].compglyph[ci].xTranslate = (~TempGlyphStorage[g].compglyph[ci].xTranslate); 
                                                    TempGlyphStorage[g].compglyph[ci].renderflags |= 0x0001;
                                                }
                                                else { TempGlyphStorage[g].compglyph[ci].xTranslate = x; }
                                                if(flag & 0x0002 && y > 0x7F) 
                                                { 
                                                    TempGlyphStorage[g].compglyph[ci].yTranslate = (~y);
                                                    TempGlyphStorage[g].compglyph[ci].yTranslate = (~TempGlyphStorage[g].compglyph[ci].yTranslate); 
                                                    TempGlyphStorage[g].compglyph[ci].renderflags |= 0x0001;
                                                }
                                                else { TempGlyphStorage[g].compglyph[ci].yTranslate = y; }
                                            }
                                            
                                            if( flag & 0x0002 )
                                            {
                                                if( flag & 0x0004 )
                                                {
                                                    //x and y are to be rounded to the closet grid line appearantly?
                                                }
                                                else
                                                {
                                                    //No need to worry about rounding to closest grid line.
                                                }
                                            }
                                            else
                                            {
                                                //Handle the points case
                                                //This would appear to be new vertex data?
                                                printf("THERE WAS A POINTS CASE!\n");
                                            }
                                        }
                                        
                                        {//GetScaleDataProcedure()
                                            if(flag & 0x0008)
                                            {
                                                bit16 uniformscale = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&uniformscale);
                                                f32 uniformscaleinfloat = uniformscale/16384.0f; 
                                                TempGlyphStorage[g].compglyph[ci].transform.d[0][0] = uniformscaleinfloat;
                                                TempGlyphStorage[g].compglyph[ci].transform.d[1][1] = uniformscaleinfloat;
                                            }
                                            else if(flag & 0x0040)
                                            {
                                                bit16 xScale = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&xScale);
                                                bit16 yScale = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&yScale);
                                                TempGlyphStorage[g].compglyph[ci].transform.d[0][0] = xScale / 16384.0f;
                                                TempGlyphStorage[g].compglyph[ci].transform.d[1][1] = yScale / 16384.0f;
                                            }
                                            else if(flag & 0x0080)
                                            {
                                                bit16 m00 = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&m00);
                                                bit16 m01 = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&m01);
                                                bit16 m10 = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&m10);
                                                bit16 m11 = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset+=sizeof(bit16); EndianSwap16(&m11);
                                                TempGlyphStorage[g].compglyph[ci].transform.d[0][0] = m00 / 16384.0f;
                                                TempGlyphStorage[g].compglyph[ci].transform.d[0][1] = m01 / 16384.0f;
                                                TempGlyphStorage[g].compglyph[ci].transform.d[1][0] = m10 / 16384.0f;
                                                TempGlyphStorage[g].compglyph[ci].transform.d[1][1] = m11 / 16384.0f;
                                            }
                                        }
                                        
                                        {//GetAuxiliaryFlagsProcedure()
                                            if(flag & 0x0200)
                                            {
                                                TempGlyphStorage[g].compglyph[ci].renderflags |= 0x0002;
                                            }
                                            if(flag & 0x0800)
                                            {
                                                TempGlyphStorage[g].compglyph[ci].renderflags |= 0x0004;
                                            }
                                        }
                                        
                                        if(flag & 0x0020) { ci++; goto CompositeGlyphProcedure; }
                                    }
                                }
                            }
                        }
                        file_io_memory_allocation_offsetter -= glyphoffsetallocvalue; 
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x20: //hhea (MSDN: https://docs.microsoft.com/en-us/typography/opentype/spec/hhea)
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        bit16 majorVersion =  ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&majorVersion);
                        bit16 minorVersion = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&minorVersion);
                        //NOTE: these appear to be the most important sets of data here. As you know how high an ascender can go. And how low a descender can go.
                        highest_ascender = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&highest_ascender);
                        lowest_descender = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&lowest_descender);
                        linegap = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&linegap);
                        //
                        bit16 advanceWidthMax = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&advanceWidthMax);
                        s16 minLeftSideBearing = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&minLeftSideBearing);
                        s16 minRightSideBearing = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&minRightSideBearing);
                        s16 xMaxExtent = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&xMaxExtent);
                        bit16 caretSlopeRise = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&caretSlopeRise);
                        bit16 caretSlopeRun = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&caretSlopeRun);
                        bit16 caretOffset = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&caretOffset);
                        offset += sizeof(bit16) * 4;
                        bit16 metricDataFormat = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&metricDataFormat);
                        numberOfHMetrics = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&numberOfHMetrics);
                        
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x40: //hmtx (MSDN: https://docs.microsoft.com/en-us/typography/opentype/spec/hmtx)
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        struct longHorMetric{
                            bit16 advanceWidth = 0;
                            s16 lsb = 0;
                        };
                        longHorMetric* LHM = (longHorMetric*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                        file_io_memory_allocation_offsetter += sizeof(LHM) * numberOfHMetrics;
                        for(bit32 i = 0; i < numberOfHMetrics; i++ )
                        {
                            LHM[i].advanceWidth = ((bit16*)file_io_memory)[offset/sizeof(bit16)]; offset += sizeof(bit16); EndianSwap16(&LHM[i].advanceWidth);
                            LHM[i].lsb = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&LHM[i].lsb);
                        }
                        
                        s16* leftsideBearings = (s16*)&((bit8*)file_io_memory)[fileloadbytesread + file_io_memory_allocation_offsetter];
                        file_io_memory_allocation_offsetter += sizeof(s16) * (numGlyphs - numberOfHMetrics);
                        bit16 glyphsublonghorizontalmetric = numGlyphs - numberOfHMetrics;
                        for(bit32 i =0; i < glyphsublonghorizontalmetric; i++)
                        {
                            leftsideBearings[i] = ((s16*)file_io_memory)[offset/sizeof(s16)]; offset += sizeof(s16); EndianSwap16(&leftsideBearings[i]);
                        }
                        
                        for(bit16 g = 0; g < numberofDesiredGlyphs; g++)
                        {
                            if ( TempGlyphStorage[g].Unicode_GlyphIndex_InFile != 0 )
                            {
                                TempGlyphStorage[g].advanceWidth = LHM[TempGlyphStorage[g].Unicode_GlyphIndex_InFile].advanceWidth;
                                if( TempGlyphStorage[g].Unicode_GlyphIndex_InFile >= numberOfHMetrics )
                                {
                                    TempGlyphStorage[g].LeftSideBearing = leftsideBearings[TempGlyphStorage[g].Unicode_GlyphIndex_InFile];
                                }
                                else
                                {
                                    TempGlyphStorage[g].advanceWidth = LHM[TempGlyphStorage[g].Unicode_GlyphIndex_InFile].lsb;
                                }
                            }
                        }
                        
                        file_io_memory_allocation_offsetter -= sizeof(s16) * (numGlyphs - numberOfHMetrics);
                        file_io_memory_allocation_offsetter -= sizeof(LHM) * numberOfHMetrics;
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x80: //kern
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    case 0x100: //GPOS
                    {
                        bit32 checksum = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&checksum);
                        bit32 offset = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&offset);
                        bit32 length = ((bit32*)file_io_memory)[fontdatatracker/sizeof(bit32)]; fontdatatracker += sizeof(bit32); EndianSwap32(&length); 
                        
                        fontdatatracker = offsetTable[f]; requiredtablesfilled++; break;
                    }
                    default:
                    {
                        fontdatatracker++; break;
                    }
                }
            }
        }
        else
        { break; }
    }
    
    debug_printf.linegap = linegap; debug_printf.lowest_descender = lowest_descender; debug_printf.highest_ascender = highest_ascender;
    
    
    //Setlongtermfontmemory()
    bit32 b = 0;
    for(bit32 g= 0; g < numberofDesiredGlyphs; g++)
    {
        if(TempGlyphStorage[g].Unicode_GlyphIndex_InFile != 0)
        {
            if(TempGlyphStorage[g].Vertex_Data != nullptr) //If simple glyph.
            {
                ((bit32*)memory_to_write_to)[b/sizeof(bit32)] = TempGlyphStorage[g].Unicode_CodePoint; b+=sizeof(bit32);
                ((s32*)memory_to_write_to)[b/sizeof(s32)] = TempGlyphStorage[g].numberofcontours; b+=sizeof(s32);
                s16 xMax = TempGlyphStorage[g].BBxMax - TempGlyphStorage[g].BBxMin;
                s16 yMax = TempGlyphStorage[g].BByMax - TempGlyphStorage[g].BByMin;
                //((s16*)memory_to_write_to)[b/sizeof(s16)] = xMax; b+=sizeof(s16);
                //((s16*)memory_to_write_to)[b/sizeof(s16)] = yMax; b+=sizeof(s16);
                //((s16*)memory_to_write_to)[b/sizeof(s16)] = TempGlyphStorage[g].LeftSideBearing; b+=sizeof(s16);
                //((bit16*)memory_to_write_to)[b/sizeof(bit16)] = TempGlyphStorage[g].advanceWidth; b+=sizeof(bit16);
                //((bit16*)memory_to_write_to)[b/sizeof(bit16)] = (float)TempGlyphStorage[g].advanceWidth / (float)(xMax + TempGlyphStorage[g].advanceWidth); b+=sizeof(bit16);
                if(TempGlyphStorage[g].numberofcontours > 0)
                {
                    bit32* runtimememorycontourdata = &((bit32*)memory_to_write_to)[b/sizeof(bit32)];
                    for(bit16 i = 0; i < TempGlyphStorage[g].numberofcontours; i++)
                    { 
                        runtimememorycontourdata[i] = TempGlyphStorage[g].Contour_Data[i]; b+=sizeof(bit32); 
                    }
                    vec3* vertexdata = (vec3*)&((bit8*)memory_to_write_to)[b];
                    
                    { //Write |vertexdata.xy|winding order|contour id?|
                        bit32 n = 0; bit32 v = 0; bit16 cfa = 0; 
                        bit32 numberofcontours = TempGlyphStorage[g].numberofcontours;
                        for(bit32 c = 0; c < numberofcontours; c++)
                        {
                            bit32 maxbasevertexes = TempGlyphStorage[g].Contour_Data[c];
                            if(maxbasevertexes == 0){goto loopend;} //Just check to ensure the first contour doesn't have 0. Appearantly one kanji has a 0 contour in it... 
                            
                            bit32 l = n; bool32 wasoffcurve = false; 
                            for(v; v < maxbasevertexes; v++)
                            {
                                f32 x = ((TempGlyphStorage[g].Vertex_Data[v].x - TempGlyphStorage[g].BBxMin)) / xMax;
                                f32 y = ((TempGlyphStorage[g].Vertex_Data[v].y - TempGlyphStorage[g].BByMin)) / yMax;
                                if( !(TempGlyphStorage[g].Controlpointflags[n/32] & (1<<cfa)) )
                                {
                                    if(n == l){ goto normalvertexwrite; }
                                    wasoffcurve = true;
                                    interpolateroutine:
                                    vertexdata[n].x = f_lerp(vertexdata[n-1].x, x, 0.5f);
                                    vertexdata[n].y = f_lerp(vertexdata[n-1].y, y, 0.5f);
                                    vertexdata[n].z = c;
                                    for(bit32 a = c; a < numberofcontours; a++) { runtimememorycontourdata[a]++; }
                                    n++;
                                }
                                else { if(wasoffcurve){wasoffcurve = false; goto interpolateroutine;} }
                                
                                normalvertexwrite:
                                vertexdata[n].x = x;
                                vertexdata[n].y = y;
                                vertexdata[n].z = c;
                                n++;
                                cfa++; if(cfa == 32){cfa = 0;}
                            }
                            
                            loopend:;
                        }
                        b+= n*sizeof(vec3);
                    }
                    
                }
                else {} //Doesn't have any vertex data, the only relevant data for this "glyph" aka spacebar. Is the advance data!
                debug_printf.debugnumberofstoredglyphs++;
            }
            else {} //Currently don't handle composite glyphs.
        }
    }
    
    {//Jank fix "ー" カタカナ.
        bit32 c = 0; void* fontdata = memory_to_write_to;
        for(bit32 g = 0; g < debug_printf.debugnumberofstoredglyphs; g++)
        {
            bit32 codepoint = ((bit32*)fontdata)[c/sizeof(bit32)]; c+=sizeof(bit32);
            s32 numcontours = ((s32*)fontdata)[c/sizeof(s32)]; c+= sizeof(s32);
            if(codepoint == 0x30FC)
            {
                //EXCEPTIONAL: this only works on 1 contour glyphs obvisouly as i'm only doing it on this one case.
                bit32 numvertexes = ((bit32*)fontdata)[c/sizeof(bit32)]; c+=sizeof(bit32); 
                vec3* vd = (vec3*)&((bit8*)fontdata)[c];
                vd[0].y = 0.4f;
                vd[1].y = 0.4f;
                
                vd[2].y = 0.6f;
                vd[3].y = 0.6f;
                break;
            }
            else
            {
                if(numcontours > 0)
                { 
                    c+=numcontours*sizeof(bit32);
                    c+=((bit32*)fontdata)[(c/sizeof(bit32))-1] * sizeof(vec3); //-1 to get the highest contour value which is the total number of vertexes.
                }
                else if(numcontours < 0)
                { c+= ((bit32*)fontdata)[c/sizeof(bit32)]*sizeof(compositeglyphdata); c+=sizeof(bit32); }
            }
        }
    }
    
    return b;
}

void my_printf(const char* string, ...)
{
    bit32* glyphlist = &debug_printf.prntfRenderList[debug_printf.debugrenderlistindex];
    va_list vl;
    va_start(vl, string);
    bit32 i = 0; bit32 gw = 0;
    loop:
    bit8 A = string[i]; bit8 B = string[i+1]; bit8 C = string[i+2];
    if( (A & 0xF0) == 0xE0 && (B & 0x80) == 0x80  && (C & 0x80) == 0x80 )
    {
        glyphlist[gw] = ( ((A & 0x0F) << 12) | ((B & 0x3F) << 6) | (C & 0x3F) );
        i+=3; gw++;
    }
    else if( (A & 0xF0) == 0xC0 && (B & 0x80) == 0x80 )
    {
        glyphlist[gw] = ( ((A & 0x1F)<<6) | (B & 0x3F) ) ;
        i +=2; gw++;
    }
    else
    {
        if(A == '%')
        {
            i++;
            switch(B)
            {
                case 's': 
                {
                    {//Do unicode codepoint proc that doesn't check for %formats()
                        bit8* es = va_arg(vl, bit8*); bit32 z = 0;
                        nonformatstrproc:
                        bit8 A = es[z]; bit8 B = es[z+1]; bit8 C = es[z+2];
                        if( (A & 0xF0) == 0xE0 && (B & 0x80) == 0x80  && (C & 0x80) == 0x80 )
                        {
                            glyphlist[gw] = ( ((A & 0x0F) << 12) | ((B & 0x3F) << 6) | (C & 0x3F) );
                            z+=3; gw++;
                        }
                        else if( (A & 0xF0) == 0xC0 && (B & 0x80) == 0x80 )
                        {
                            glyphlist[gw] = ( ((A & 0x1F)<<6) | (B & 0x3F) );
                            z+=2; gw++;
                        }
                        else
                        {
                            glyphlist[gw] = A;
                            z++; gw++;
                        }
                        if(es[z] != 0) { goto nonformatstrproc; }
                    }
                }
                case 'u':
                {
                    {//Find ascii character from bit32 -> bit8 proc()
                        bit32 value = 0;
                        value = va_arg(vl, bit32);
                        bit32 t = 1;
                        for(t;t<=100000000;t*=10){ if(value/(t*10) == 0) {break;} }
                        for(t; t > 0; t/=10)
                        {
                            bit32 d = value / t;
                            glyphlist[gw] = integertocodepoint(d); gw++; value%=t;
                        }
                    }
                    break;
                }
                case 'i':
                {
                    {//Find ascii character from bit32 -> bit8 proc()
                        bit32 value = 0;
                        value = va_arg(vl, bit32);
                        if(value & 0x80000000)
                        {
                            glyphlist[gw] = 0x30DE; gw++;
                            glyphlist[gw] = 0x30A4; gw++;
                            glyphlist[gw] = 0x30CA; gw++;
                            glyphlist[gw] = 0x30B9; gw++;
                            value = -(value);
                        }
                        else
                        {
                            glyphlist[gw] = 0x30D4; gw++;
                            glyphlist[gw] = 0x30E9; gw++;
                            glyphlist[gw] = 0x30B9; gw++;
                            glyphlist[gw] = ' '; gw++;
                        }
                        bit32 t = 1;
                        for(t;t<=100000000;t*=10){ if(value/(t*10) == 0) {break;} }
                        for(t; t > 0; t/=10)
                        {
                            bit32 d = value / t;
                            glyphlist[gw] = integertocodepoint(d); gw++; value%=t;
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            glyphlist[gw] = A; gw++;
        }
        i++;
    }
    if(string[i] != 0) { goto loop; }
    va_end(vl);
    debug_printf.debugrenderlistindex += gw;
}

void render_printf()
{
    bit32* glyphlist = &debug_printf.prntfRenderList[0];
    void* fontdata = debug_printf.prntfFontData;
    
    bit32 sizeofvertexbuffer = 8192*sizeof(vec3);  //NOTE: if you change the max buffer size, change in shader too!
    vec4* vertexbuffer = (vec4*)stackpush(sizeofvertexbuffer); bit32 v = 0;
    f32 currentpos = 0; f32 baseline = 0.95f; bit32 contouroffset = 0;
    
    for(bit32 g = 0; g < debug_printf.debugrenderlistindex; g++)
    {
        bit32 b = 0;
        for(bit32 gd = 0; gd < debug_printf.debugnumberofstoredglyphs; gd++ )
        {
            bit32 codepoint = ((bit32*)fontdata)[b/sizeof(bit32)]; b+=sizeof(bit32);
            s32 numcontours = ((s32*)fontdata)[b/sizeof(s32)]; b+=sizeof(s32);
            
            if( glyphlist[g] == 0x20)
            {
                currentpos+=0.012f; break;
            }
            else if( codepoint == glyphlist[g])
            {
                if(numcontours > 0) 
                {
                    bit32 numvertexes = 0; bit32* contourbuffer = nullptr;
                    vec3* intermediatevertexbuffer = nullptr;
                    
                    {//Get simple glyph routine()
                        {//Get contour data procedure()
                            contourbuffer = (bit32*)stackpush(numcontours*sizeof(bit32));
                            for(bit16 c = 0; c < numcontours; c++) { contourbuffer[c] = ((bit32*)fontdata)[b/sizeof(bit32)]; b+=sizeof(bit32); }
                        }
                        
                        {//Get vertex data procedure()
                            numvertexes = contourbuffer[numcontours-1];
                            bit32 i = 0;
                            intermediatevertexbuffer = (vec3*)stackpush(numvertexes*sizeof(vec3));
                            vec3* vertexdata = (vec3*)&((bit8*)fontdata)[b]; 
                            for(bit16 i = 0; i < numvertexes; i++) { intermediatevertexbuffer[i] = vertexdata[i]; b+=sizeof(vec3);}
                        }
                    }
                    
                    {//Write to final vertex buffer.
                        mat2x2 scale; float scaleamt = 0; float advanceamt = 0;
                        if(codepoint >= 0x61 && codepoint <= 0x7A)
                        { scaleamt = 0.007f; advanceamt = 0.008f; }
                        else { scaleamt = 0.011f; advanceamt = 0.012f; }
                        scale.d[0][0] = scaleamt; scale.d[0][1] = 0;
                        scale.d[1][0] = 0; scale.d[1][1] = scaleamt;
                        float lastz = intermediatevertexbuffer[0].z;
                        for(bit32 i = 0; i < numvertexes; i++, v++)
                        {
                            vec2 scaled;
                            scaled.x = intermediatevertexbuffer[i].x;
                            scaled.y = intermediatevertexbuffer[i].y;
                            scaled = scale * scaled;
                            
                            vertexbuffer[v].x = scaled.x + currentpos; 
                            vertexbuffer[v].y = scaled.y + baseline;
                            if(lastz != intermediatevertexbuffer[i].z){lastz = intermediatevertexbuffer[i].z; contouroffset++; }
                            vertexbuffer[v].z = contouroffset;
                            vertexbuffer[v].w = 0;
                        }
                        //currentpos += (float)((float)advanceWidth/(float)(xMax+advanceWidth));
                        currentpos += advanceamt;
                        contouroffset++;
                    }
                    stackpop(numvertexes*sizeof(vec3));
                    stackpop(numcontours*sizeof(bit32));
                }
                else if(numcontours == 0)
                {
                    //NUll term.
                }
                else if(numcontours < 0)
                {
                    //Not handling the composite glyphs atm. There's only one. And that glyph is the "\" glyph. (REALLY BACKSLASH BUT UNICODE MAPPING FOR KEYCODE IS YEN!)
                }
                
                break;
            }
            else if ( glyphlist[g] == '\n')
            {
                baseline -=.015f; currentpos = 0; break;
            }
            else
            {
                //b+=sizeof(s16)*4; 
                if(numcontours > 0)
                { 
                    b+=numcontours*sizeof(bit32);
                    b+=((bit32*)fontdata)[(b/sizeof(bit32))-1] * sizeof(vec3); //-1 to get the highest contour value which is the total number of vertexes.
                }
                else if(numcontours < 0)
                { b+= ((bit32*)fontdata)[b/sizeof(bit32)]*sizeof(compositeglyphdata); b+=sizeof(bit32); }
            }
        }
    }
    
    {//Renderproc()
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, debug_printf.prntfSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, v*sizeof(vec4), vertexbuffer);
        glUseProgram(debug_printf.prntfProgram);
        glUniform1ui(0, v);
        glUniform1f(1, baseline);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    
    stackpop(sizeofvertexbuffer);
    debug_printf.debugrenderlistindex = 0;
}