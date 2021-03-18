struct OfflineSettings
{
    bit32 refreshrate;
    bit16 volume; bit16 whitenoisecompensation;
};
struct Settings
{
    bit32 windowresWIDTH = defaultwindowresWIDTH; bit32 windowresHEIGHT = defaultwindowresHEIGHT;
    bit32 refreshrate = defaultrefreshrate; bit16 volume = defaultvolume; bit16 whitenoisecompensation = defaultwhitenoisecompensation;
    bit32 audiosamplesperframe = defaultaudiosamplesperframe;
    bit32 target_timeperframe = 1000/defaultrefreshrate;
    bit32 audiosamplerate = defaultaudiosamplerate; bit32 micsamplerate = defaultaudiochannels;
    bit32 audiochannels = defaultaudiochannels;
};

struct SettingsController
{
    bool32 isactive; bit32 inputdelay;
    VertexArenaVector vertex[12];
    bit16 indexbuffer[24];
    VertexArenaVector icon[23];
    bit16 iconindexbuffer[51];
}; //Logic behind this~ I want the settings to be stored inside the binaries of the game. I'd rather have this controller separate at the moment. Aka, the baggage. As the pointer will be in the binaries.

Settings settings;//NOTE: global settings struct
SettingsController* settingscontroller; //NOTE: global settings controller pointer

void loadsettings()
{
    {//LoadOfflineSettings()
        bit32 settingsalloc = sizeof(OfflineSettings);
        OfflineSettings* file = (OfflineSettings*)stackpush(settingsalloc);
        if(platform_fileload("Settings.JJsettings", file) == 0)
        {
            file->refreshrate = defaultrefreshrate;
            file->volume = defaultvolume; file->whitenoisecompensation = defaultwhitenoisecompensation;
            platform_filewrite("Settings.JJsettings", file, settingsalloc);
        }
        settings.refreshrate = file->refreshrate;
        settings.volume = file->volume; settings.whitenoisecompensation = file->whitenoisecompensation;
        settings.audiosamplesperframe = settings.audiosamplerate / settings.refreshrate;
        settings.target_timeperframe = (1000/settings.refreshrate);
        stackpop(settingsalloc);
    }
    
    f32 z = -.1;
    {//LoadSettingsGraphics()
        settingscontroller = (SettingsController*)recordpush(sizeof(SettingsController));
        settingscontroller->isactive = 0; settingscontroller->inputdelay = minframesforinput;
        bit32 v = 0;
        f32 lowX = -.4; f32 maxX = .4;
        f32 normalizedvolume = (f32)settings.volume / (f32)maxsignedsixteenvalue;
        f32 volX = (normalizedvolume * (maxX - lowX)) - maxX;
        settingscontroller->vertex[v].position = {maxX, .5, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++;
        settingscontroller->vertex[v].position = {maxX, .1, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++;
        settingscontroller->vertex[v].position = {volX, .1, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++; //Middle bottom
        settingscontroller->vertex[v].position = {lowX, .1, z}; settingscontroller->vertex[v].color = {1, 1, 1, 1}; v++;
        settingscontroller->vertex[v].position = {lowX, .5, z}; settingscontroller->vertex[v].color = {1, 1, 1, 1}; v++;
        settingscontroller->vertex[v].position = {volX, .5, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++; //Middle top
        f32 normalizedwhitenoise = (f32)settings.whitenoisecompensation / (f32)maxsignedsixteenvalue;
        f32 wnX = (normalizedwhitenoise * (maxX - lowX)) - maxX;
        settingscontroller->vertex[v].position = {maxX, -.1, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++;
        settingscontroller->vertex[v].position = {maxX, -.5, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++;
        settingscontroller->vertex[v].position = {wnX, -.5, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++; //Middle bottom
        settingscontroller->vertex[v].position = {lowX, -.5, z}; settingscontroller->vertex[v].color = {1, 1, 1, 1}; v++;
        settingscontroller->vertex[v].position = {lowX, -.1, z}; settingscontroller->vertex[v].color = {1, 1, 1, 1}; v++;
        settingscontroller->vertex[v].position = {wnX, -.1, z}; settingscontroller->vertex[v].color = {.7, .7, .7, 1}; v++; //Middle top
        Assert(v == sizeof(settingscontroller->vertex)/(sizeof(vec3)+sizeof(vec4)));
        bit32 i = 0;
        settingscontroller->indexbuffer[i] = 0;  i++; settingscontroller->indexbuffer[i] = 1;  i++; settingscontroller->indexbuffer[i] = 2;  i++;
        settingscontroller->indexbuffer[i] = 2;  i++; settingscontroller->indexbuffer[i] = 5;  i++; settingscontroller->indexbuffer[i] = 0;  i++;
        settingscontroller->indexbuffer[i] = 2;  i++; settingscontroller->indexbuffer[i] = 3;  i++; settingscontroller->indexbuffer[i] = 5;  i++;
        settingscontroller->indexbuffer[i] = 3;  i++; settingscontroller->indexbuffer[i] = 4;  i++; settingscontroller->indexbuffer[i] = 5;  i++;
        settingscontroller->indexbuffer[i] = 6;  i++; settingscontroller->indexbuffer[i] = 7;  i++; settingscontroller->indexbuffer[i] = 8;  i++;
        settingscontroller->indexbuffer[i] = 8;  i++; settingscontroller->indexbuffer[i] = 11;  i++; settingscontroller->indexbuffer[i] = 6;  i++;
        settingscontroller->indexbuffer[i] = 8;  i++; settingscontroller->indexbuffer[i] = 9;  i++; settingscontroller->indexbuffer[i] = 11;  i++;
        settingscontroller->indexbuffer[i] = 9;  i++; settingscontroller->indexbuffer[i] = 10;  i++; settingscontroller->indexbuffer[i] = 11;  i++;
        Assert(i == sizeof(settingscontroller->indexbuffer)/sizeof(bit16));
    }
    {//Load Audio and Mic Icon
        bit32 v = 0;
        {//Audio Icon Vertex Data()
            settingscontroller->icon[v].position = {.02, .55, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++; //0
            settingscontroller->icon[v].position = {-.02, .58, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.03, .58, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.035, .59, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.04, .6, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.035, .61, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.03, .62, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.02, .62, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {.02, .65, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++; //8
        }
        {//Mic Icon Vertex Data
            //Upper half
            settingscontroller->icon[v].position = {.02, .005, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++; //9
            settingscontroller->icon[v].position = {.02, .03, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {.01, .035, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {0, .04, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.01, .035, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.02, .03, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.02, .005, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++; //15
            //Lower half
            settingscontroller->icon[v].position = {.02, -.005, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;//16
            settingscontroller->icon[v].position = {.02, -.03, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {.01, -.035, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {0, -.04, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.01, -.035, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.02, -.03, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;
            settingscontroller->icon[v].position = {-.02, -.005, z}; settingscontroller->icon[v].color = {1, 1, 1, 1}; v++;//22
        }
        bit32 i = 0;
        {//Audio Icon Index Data()
            settingscontroller->iconindexbuffer[i] = 8;i++; settingscontroller->iconindexbuffer[i] = 0;i++; settingscontroller->iconindexbuffer[i] = 1;i++;
            settingscontroller->iconindexbuffer[i] = 8;i++; settingscontroller->iconindexbuffer[i] = 1;i++; settingscontroller->iconindexbuffer[i] = 7;i++;
            settingscontroller->iconindexbuffer[i] = 7;i++; settingscontroller->iconindexbuffer[i] = 1;i++; settingscontroller->iconindexbuffer[i] = 2;i++;
            settingscontroller->iconindexbuffer[i] = 7;i++; settingscontroller->iconindexbuffer[i] = 2;i++; settingscontroller->iconindexbuffer[i] = 3;i++;
            settingscontroller->iconindexbuffer[i] = 7;i++; settingscontroller->iconindexbuffer[i] = 3;i++; settingscontroller->iconindexbuffer[i] = 4;i++;
            settingscontroller->iconindexbuffer[i] = 7;i++; settingscontroller->iconindexbuffer[i] = 4;i++; settingscontroller->iconindexbuffer[i] = 5;i++;
            settingscontroller->iconindexbuffer[i] = 7;i++; settingscontroller->iconindexbuffer[i] = 5;i++; settingscontroller->iconindexbuffer[i] = 6;i++;
        }
        {//Mic Icon Index Data()
            //Upper half
            settingscontroller->iconindexbuffer[i] = 10;i++; settingscontroller->iconindexbuffer[i] = 9;i++; settingscontroller->iconindexbuffer[i] = 15;i++;
            settingscontroller->iconindexbuffer[i] = 10;i++; settingscontroller->iconindexbuffer[i] = 15;i++; settingscontroller->iconindexbuffer[i] = 14;i++;
            settingscontroller->iconindexbuffer[i] = 11;i++; settingscontroller->iconindexbuffer[i] = 10;i++; settingscontroller->iconindexbuffer[i] = 14;i++;
            settingscontroller->iconindexbuffer[i] = 11;i++; settingscontroller->iconindexbuffer[i] = 14;i++; settingscontroller->iconindexbuffer[i] = 13;i++;
            settingscontroller->iconindexbuffer[i] = 12;i++; settingscontroller->iconindexbuffer[i] = 11;i++; settingscontroller->iconindexbuffer[i] = 13;i++;
            //Lower half
            settingscontroller->iconindexbuffer[i] = 16;i++; settingscontroller->iconindexbuffer[i] = 17;i++; settingscontroller->iconindexbuffer[i] = 21;i++;
            settingscontroller->iconindexbuffer[i] = 16;i++; settingscontroller->iconindexbuffer[i] = 21;i++; settingscontroller->iconindexbuffer[i] = 22;i++;
            settingscontroller->iconindexbuffer[i] = 17;i++; settingscontroller->iconindexbuffer[i] = 18;i++; settingscontroller->iconindexbuffer[i] = 20;i++;
            settingscontroller->iconindexbuffer[i] = 17;i++; settingscontroller->iconindexbuffer[i] = 20;i++; settingscontroller->iconindexbuffer[i] = 21;i++;
            settingscontroller->iconindexbuffer[i] = 18;i++; settingscontroller->iconindexbuffer[i] = 19;i++; settingscontroller->iconindexbuffer[i] = 20;i++;
        }
    }
}