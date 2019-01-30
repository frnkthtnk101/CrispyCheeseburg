import os

def child(arg_items):
    os.execvp(arg_items[0],arg_items[1::1])
    os._exit(0)


def parent():
    i = 0
    while True:
        args = (input("osh>")).split(" ")
        if args[0] != "exit":
            pid = os.fork()
            if pid > 0:
                if "&" in args:
                    os.wait()
            if pid == 0:
                child(args)

        else:
            break

    


parent()
