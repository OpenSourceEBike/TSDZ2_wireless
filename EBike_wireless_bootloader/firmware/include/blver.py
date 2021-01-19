# Help python tool to convert a semantic version string to hex for nrfutil --bootloader-version parameter
# This converts a version like 0.1.0 to 0x3e8 and viceversa
import argparse

# Control entrypoint
def run(args):
    if (args.version == True):
        return version()
    elif (args.toHex != None):
        return toHex(args.toHex[0])
    elif (args.toSemVer != None):
        return toSemVer(args.toSemVer[0])
    elif(args.toStringVer != None):
        return toStringVer(args.toStringVer[0])
    else:
        return version()

def version():
    """Returns the blever version"""
    return "blver: 0.1.0"

def toHex(version):
    """Converts a semantic version string to a hex string"""
    split = version.split(".")
    fullversion = ""
    for i, v in enumerate(split, start=0):
        fullversion += v.rjust(3, "0")

    return hex(int(fullversion))

def toSemVer(hex):
    fullversion = str(int(hex, 0)).rjust(9, "0")
    split = [fullversion[i:i+3] for i in range(0, len(fullversion), 3)]
    return str(int(split[0])) + "." + str(int(split[1])) + "." + str(int(split[2]))

def toStringVer(version):
    """Converts a semantic version string to a string with leading zeros"""
    split = version.split(".")
    fullversion = ""
    for i, v in enumerate(split, start=0):
        fullversion += v.rjust(2, "0")


    return fullversion

parser = argparse.ArgumentParser(description='Converts a semantic version string to hex and back')
parser.add_argument('--version', help='Specifies the version to use', action="store_true")
parser.add_argument('--toHex', nargs="+", help='Converts a semantic version string to hex')
parser.add_argument('--toSemVer', nargs="+", help='Converts a hex version string to a semantic version string')
parser.add_argument('--toStringVer', nargs="+", help='Converts a semantic version string to one with leading zeros')
args = parser.parse_args()
print(run(args))