Import('env')
from base64 import b64decode

#
# Dump build environment (for debug)
# print env.Dump()
#

env.Replace(UPLOADHEXCMD='"$UPLOADER" ' + b64decode(ARGUMENTS.get("CUSTOM_OPTION")) + ' --uploader --flags')

# uncomment line below to see environment variables
# print ARGUMENTS