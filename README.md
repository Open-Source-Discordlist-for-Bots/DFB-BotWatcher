# CDL++ template

## Cloning
Issue `git submodule update --init --recursive --depth 1` after cloning so submodules are checked out

## Creating config.json
For the configuration step to work, you have to create a `config.json` first:

    {
        "token": "<bot token>"
    }

you can add your own settings and access them via the `env.settings` object

## Updating CDL
Run `git pull` inside `lib/cdlpp`

## Building

    mkdir build
    cd build
    cmake .. # you might have to run this twice
    make -j$(nproc)

