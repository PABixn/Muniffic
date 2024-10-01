import sys
import subprocess
import importlib.util as importlib_util

class PythonConfiguration:
    @classmethod
    def Validate(cls):
        if not cls.__ValidatePython():
            return # cannot validate further
        
        packageNames = {
            "requests":"requests", 
            "openai":"openai", 
            "typing":"typing" ,
            "pyttsx3":"pyttsx3", 
            "pyaudio":"pyaudio",
            "SpeechRecognition":"speech_recognition", 
            "python-dotenv":"dotenv"
            }

        for installName, packageName in packageNames.items():
            print(installName)
            if not cls.__ValidatePackage(installName, packageName):
                return # cannot validate further

    @classmethod
    def __ValidatePython(cls, versionMajor = 3, versionMinor = 3):
        if sys.version is not None:
            print("Python version {0:d}.{1:d}.{2:d} detected.".format(
                sys.version_info.major, sys.version_info.minor, sys.version_info.micro))
            if sys.version_info.major < versionMajor or (sys.version_info.major == versionMajor and sys.version_info.minor < versionMinor):
                print("Python version too low, expected version {0:d}.{1:d} or higher.".format( \
                    versionMajor, versionMinor))
                return False
            return True
       

    @classmethod
    def __ValidatePackage(cls, installName, packageName):
        if importlib_util.find_spec(packageName) is None:
            return cls.__InstallPackage(installName, packageName)
        return True

    @classmethod
    def __InstallPackage(cls, installName, packageName):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install Python package '{0:s}'? [Y/N]: ".format(installName))).lower().strip()[:1]
            if (reply == 'n' or reply == 'N'):
                return False
            permissionGranted = (reply == 'y' or reply == 'Y')
        
        print(f"Installing {installName} module...")
        subprocess.check_call(['python', '-m', 'pip', 'install', installName])
        return cls.__ValidatePackage(installName, packageName)

if __name__ == "__main__":
    PythonConfiguration.Validate()
input("zak")