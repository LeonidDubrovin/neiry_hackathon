from ui import SOUND_FILE, feedback_field, concentration_text, root
import sounddevice as sd
import soundfile as sf


async def play_sound():
    sd.play(sf.read(SOUND_FILE))
    feedback_field.set("Звук воспроизведён")


# Пока не знаю, откуда брать концентрацию, посему оставлю цикл как есть
async def start_loop():  #TODO
    while True:

        # Текст Label в Tkinter можно изменить только командой config
        concentration_text.config(master=root, borderwidth=2, background="#FFFFFF", relief="solid", text=f"{concentration}", font="Verdana 8")

        # Здесь должна быть обработка показателя концентрации
        if concentration < 30: #TODO
            play_sound()
