struct GameMemory
{
    bit64 maxsize = Megabytes(500);
    bit64 stackposition = Megabytes(500);
    bit64 recordposition = 0;
    bit64 baseaddress;
};

GameMemory gamememory; //NOTE global for game memory chunk.

#define recordpush(sizeofdata) (gamememory.baseaddress+gamememory.recordposition); (gamememory.recordposition+=sizeofdata); Assert(gamememory.recordposition < gamememory.stackposition);
#define stackpush(sizeofdata) ((gamememory.baseaddress+gamememory.stackposition) - sizeofdata); (gamememory.stackposition-=sizeofdata); Assert(gamememory.stackposition > gamememory.recordposition);
#define stackpop(sizeofdata) (gamememory.stackposition+=sizeofdata); Assert(gamememory.stackposition <= gamememory.maxsize);