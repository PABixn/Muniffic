import speech_recognition as sr
import pyttsx3

def SpeakText(command):
    engine = pyttsx3.init()
    engine.say(command)
    engine.runAndWait()

def StartVoiceAssistant():
    r = sr.Recognizer()

    while (1):

        try:
            with sr.Microphone() as source2:
                r.adjust_for_ambient_noise(source2, duration=0.2)

                audio2 = r.listen(source2)

                MyText = r.recognize_google(audio2)
                MyText = MyText.lower()

                print(MyText)

                if MyText.lower() == "hello bob".lower():
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

        except sr.RequestError as e:
            print("Could not request results")
            return "erorr"

        except sr.UnknownValueError :
            print("Unknown error occurred")
            return "error"