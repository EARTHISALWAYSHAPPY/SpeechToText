from Proto.text_with_prefix.wake_listener import WakeWordListener
from Proto.text_with_prefix.command_listener import listen_for_command

def main():
    wake = WakeWordListener()

    while True:
        wake.listen_for_wake()

        command = listen_for_command()
        if command:
            print("✅ FINAL COMMAND:", command)
        else:
            print("⚠️ No command detected")

if __name__ == "__main__":
    main()
