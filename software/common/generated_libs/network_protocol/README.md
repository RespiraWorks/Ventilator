   MacOS:
    
    brew install nanopb-generator
    $ pip3 install protobuf
     NANOPB_PATH=$(brew --cellar nanopb-generator)/$(brew list --versions \
          nanopb-generator | awk '{print $2}')
     protoc \
        --plugin=$NANOPB_PATH/bin/protoc-gen-nanopb \
        -I $NANOPB_PATH/libexec/proto \
        -Icommon/generated_libs/network_protocol \
        --nanopb_out=common/generated_libs/network_protocol \
        --python_out=utils \
        common/generated_libs/network_protocol/network_protocol.proto

   Linux:
    
    sudo apt install nanopb
    pip3 install protobuf
    protoc \
      --plugin=usr/bin/protoc-gen-nanopb     
      -I usr/lib/python3/dist-packages/proto \
      -Icommon/generated_libs/network_protocol \
      --nanopb_out=common/generated_libs/network_protocol \
      --python_out=utils \
      common/generated_libs/network_protocol/network_protocol.proto

