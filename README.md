# AllDebrid cli

## Prerequisite
This project currently supports [AllDebrid](alldebrid.com) service. So you need an API key for it saved in environment variable named `ALLDEBRID_API_KEY`. 

## Build
This project uses cmake to build. Run following commands:
```
mkdir -p build
cd build
cmake ..
cmake --build .
```

In order to install it, run following after you build it:
```
cmake --install .
```

## Usages

### Unlock link
```
adcli <link>
```

You can also pipe the links:
```
<some other command> | adcli
```

### Save link
```
adcli --save <link>
```
