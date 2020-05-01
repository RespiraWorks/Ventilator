
Import( "env" )

env.Append(
  LINKFLAGS=[
     "-mfpu=fpv4-sp-d16",
     "-mfloat-abi=hard"
  ]
)
