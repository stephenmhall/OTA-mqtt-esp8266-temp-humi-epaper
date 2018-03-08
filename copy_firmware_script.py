Import("env")
from shutil import copyfile

#print env.Dump()

print env['BUILD_FLAGS']
ota_folder = env['BUILD_FLAGS'][2].split('=')[-1]


print env['CPPDEFINES']
firmware_folder = env['PROJECTBUILD_DIR']
firmware_board = env['BOARD']
firmware_name = env['PROGNAME'] + ".bin"
firmware_version = env['BUILD_FLAGS'][1].split('=')[-1]
unit_name = env['BUILD_FLAGS'][0].split('=')[-1]

firmware_source = firmware_folder + "\\" + firmware_board + "\\" + firmware_name

print "Copying \n" + firmware_source + " \nto " + ota_folder
#print firmware_source
#print "to"
#print ota_folder

def copyFirmware(src, dest):
    copyfile(src, dest)

def writeVersion(dest, version):
    print "writing version file"
    with open(dest, 'w') as f:
        f.write(version)

def before_upload(source, target, env):
    print "before upload"
    copyFirmware(firmware_source, ota_folder + firmware_name)
    writeVersion((ota_folder + unit_name + ".version"),firmware_version)


env.AddPreAction("upload", before_upload)
