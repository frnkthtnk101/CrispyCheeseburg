'''
FPTerminal.py
Homework 2 part 1 and part 2 and extra assignments
A concept shell
Franco Pettigrosso
'''
import os #used to run fork, execvp
import re #make sure the user is calling ! command correctly

'''
run_command
This is what the child process runs when the parent is forked
'''
def run_command(arg_items):
    os.execvp(arg_items[0],arg_items[1::1])
    os._exit(0)

'''
show last commands
shows the elements in a list that is given
'''
def show_last_comands(commands):
    commands_length = len(commands)
    #I reversed it to show the latest command first is the
    #first command the user sees
    for i  in range(commands_length,0, -1):
        if i >= 0:
            print(f"{i} {commands[i]}")

'''
start_shell
what the main function uses. to run the shell
'''
def start_shell():
    regex  =  re.compile('\!\d+')
    last_commands = []
    while True:
        args = []
        given_command = input("osh>")
        if given_command is None or given_command is '':
            continue
        elif given_command == '!!':
            show_last_comands(last_commands)
            continue
        elif regex.match(given_command):
            #need to get ring of the '!' the given command
            #can be converted into an int!
            index = (int) (given_command.replace("!",""))
            args = last_commands[index].split(' ')
        else:
            last_commands.append(given_command)
            args = last_commands[-1].split(' ')
        if args[0] != "exit":
            pid = os.fork()
            if pid > 0:
                if "&" in args:
                    os.wait()
            if pid == 0:
                #if we leave the '&' in there
                #the program errors
                if "&" in args:
                    run_command(args[0:-1])
                else:
                    run_command(args)
        else:
            break
        if len(last_commands) > 11:
            last_commands.pop(0)
'''
main
driver function
'''
def main():
    start_shell()

main()