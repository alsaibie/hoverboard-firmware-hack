Import('env')
from base64 import b64decode


print "Unlock Target!"
env.Execute('openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c init -c "reset halt" -c "stm32f1x unlock 0"')

