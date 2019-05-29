# SimpleRemoteDesktop client

client application based on sdl2

## requirements

init all submodule :
```
git submodule update --init --recursive
```

ubuntu package : 

```bash
sudo apt-get install build-essential libavutil-dev libavcodec-dev libswscale-dev libopus-dev libsdl2-dev libavformat-dev build-essential libavutil-dev libavcodec-dev libswscale-dev libopus-dev libsdl2-dev libsdl2-net-dev libavformat-dev cmake libva-dev yasm

```

cross compile build ofr windows:
 
```bash
sudo apt-get install mingw-w64
cmake -DCMAKE_TOOLCHAIN_FILE=WindowsToolChain.cmake .
make distZip // generate zip archive
```

## build for windows


## run the client

```bash
./client hostname port WxH 10000000 30
```

### arguments list
1. hostname : distant server hostname or ip.(required);
2. port : distant server tcp port. (required);
3. video resolution : Width and height separated by 'x' ex: 1920x1200)
4. bandwidth : bandwidth of the video stream in bit( optional, default : 10Mbit )
5. framerate : video refreash rate in frame by second( optional, default : 25 );

