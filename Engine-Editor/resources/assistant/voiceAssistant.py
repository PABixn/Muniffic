import speech_recognition as sr
import pyttsx3
import pyaudio

shouldListen = False
engine = pyttsx3.init()

def InitVoiceAssistant():
    r = sr.Recognizer()
    global engine
    engine = pyttsx3.init()
    voices = engine.getProperty('voices')

    for voice in voices:
        if 'english' in voice.name.lower() and "david" in voice.name.lower():
            engine.setProperty('voice', voice.id)
            break


def CheckMicrophoneAvailable():
    audio = pyaudio.PyAudio()

    try:
        default_device_index = audio.get_default_input_device_info()
        #print(f"Default microphone is available: {default_device_index['name']}")
    except IOError:
        #print("No default microphone available.")
        return False
    finally:
        audio.terminate()

    mic_list = sr.Microphone.list_microphone_names()

    if len(mic_list) > 0:
        print("Available microphones:")
        for index, microphone_name in enumerate(mic_list):
            print(f"{index}: {microphone_name}")
        return True
    else:
        print("No microphone available.")
        return False


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