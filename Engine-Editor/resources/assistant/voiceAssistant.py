import speech_recognition as sr
import pyttsx3
import pyaudio

shouldListen = False
engine = pyttsx3.init()
r = sr.Recognizer()

def InitVoiceAssistant():
    global engine
    engine = pyttsx3.init()
    voices = engine.getProperty('voices')

    for voice in voices:
        if 'english' in voice.name.lower() and "david" in voice.name.lower():
            engine.setProperty('voice', voice.id)
            break

def CheckMicrophoneAvailable():
    audio = pyaudio.PyAudio()
    is_available = False

    try:
        default_device_index = audio.get_default_input_device_info()
        if default_device_index:
            print(f"Default microphone is available: {default_device_index['name']}")
            is_available = True
    except IOError:
        print("Error: Could not access default microphone.")

    mic_list = sr.Microphone.list_microphone_names()

    if len(mic_list) > 0:
        is_available = True
    else:
        print("No microphone available.")

    if is_available:
        try:
            stream = audio.open(format=pyaudio.paInt16, channels=1, rate=44100, input=True, frames_per_buffer=1024)
            print("Microphone is functional.")
            stream.close()
        except Exception as e:
            is_available = False

    audio.terminate()

    return is_available


def SpeakText(command):
    global engine
    engine.say(command)
    engine.runAndWait()


def StopVoiceAssistant():
    global shouldListen
    shouldListen = False

def StartVoiceAssistant():
    global shouldListen
    shouldListen = True
    global r
    while shouldListen:
        try:
            with sr.Microphone() as source2:
                r.adjust_for_ambient_noise(source2, duration=0.2)

                audio2 = r.listen(source2)

                MyText = r.recognize_google(audio2)
                MyText = MyText.lower()

                if "hi bob".lower() == MyText.lower():
                    SpeakText("Hello, how can I help you?")

                    with sr.Microphone() as source:
                        print("Talk")
                        audio_text = r.listen(source)
                        print("Time over, thanks")

                        try:
                            transcribed = r.recognize_google(audio_text)
                            print("Text: " + transcribed)
                            return transcribed
                        except:
                            print("Sorry, I did not get that")
                            return "Error"
                elif "hi bob".lower() in MyText.lower():
                    result = MyText.replace("hi bob", "")
                    result.strip()
                    return result

        except sr.RequestError as e:
            print("Could not request results")

        except sr.UnknownValueError :
            print("Heard nothing")