Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
#print defines

build_tag = defines.get('UNIT_NAME') + "_" + defines.get('VERSION')
env.Replace(PROGNAME= build_tag)

print env['PROGNAME']
