import os
from tkinter import Tk, ttk, Text, END

import sounddevice as sd
import soundfile as sf

directory = "C:\\Users\\Lenovo\\Desktop\\Hackathon\\notification_app\\media"
sounds_list = os.listdir(directory)
SOUND_FILE = sounds_list[0]


# Обработчик выбора элемента из списка звуков в ComboBox
def on_combo_chosen(event):
    SOUND_FILE = combo_sound.get()
    feedback_field.insert(END, chars=f"\nВыбран звук {SOUND_FILE}")


# Инициализация списка устройств (не работает)
sound_devices = sd.query_devices() #TODO
devices_list = [f"{x.get('name')}\n" for x in sound_devices] #TODO


root = Tk()
root.title("Notification App")
root.geometry("500x400+710+340")
root.resizable(width=False, height=False)

# Фрейм для текстового поля вывода уведомлений
frame_bottom = ttk.Frame(borderwidth=0, width=480, height=190)
frame_bottom.place(x=10, y=180)

# Поле вывода уведомлений
feedback_field = Text(master=frame_bottom, bd=1, font="Verdana 8", wrap="char")
feedback_field.place(x=0, y=0)
feedback_field.insert(END, chars="Приложение запущено")

# Надпись над ComboBox со списком звуков
label_sound = ttk.Label(master=root, text="Выберите звук", font="Verdana 8")
label_sound.place(x=50, y=50)

# ComboBox со списком звуков
combo_sound = ttk.Combobox(master=root, values=sounds_list, state="readonly")
combo_sound.bind("<<ComboboxSelected>>", on_combo_chosen)
combo_sound.place(x=50, y=70)
combo_sound.current(newindex=0)

# Надпись над Label для вывода показателя концентрации
concentration_label = ttk.Label(master=root, text="Концентрация", font="Verdana 8")
concentration_label.place(x=250, y=50)

# Label для вывода показателя концентрации
concentration_text = ttk.Label(master=root, borderwidth=2, background="#FFFFFF", relief="solid", text="----", font="Verdana 8")
concentration_text.place(x=250, y=70)
