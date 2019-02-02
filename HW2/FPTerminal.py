'''
FPTerminal.py
Homework 2 part 1 and part 2 and extra assignments
A concept shell
Franco Pettigrosso
'''
import io
import os #used to run fork, execvp
import re #make sure the user is calling ! command correctly
import sys
aliases = dict()
verbose = False
regex_new_alias = re.compile('(alias)\ (\w+)\ \"([\w\ \-]+)\"')
regex_remove_alias = re.compile('(unalias)\ (\w+)')
regex_grab_paths = re.compile('set\ path\ \=\ \((.+)\)')

def toggle_chaty(command):
    global verbose
    if command == "set verbose on":
        verbose = True
        verboseprint("hello im chatty today..")
    else:
        verboseprint("goodbye!")
        verbose = False

def with_pre_commands():
    verboseprint("with precommands started")
    path = "{}/.cs543rc".format(os.path.expanduser("~"))
    verboseprint("getting path")
    temp = []
    verboseprint("seeing if that .cs543rc file exist")
    if os.path.exists(path):
        verboseprint("getting commands")
        reader = open(path,"r")
        for line in reader:
            if line.startswith("set verbose"):
                toggle_chaty(line.strip())
            else:
                temp.append(line.strip())
        reader.close
    verboseprint("done with_pre_commands")
    return temp

def set_paths(command):
    verboseprint("set path started")
    groups = regex_grab_paths.match(command)
    temp = []
    if groups:
        verboseprint("match!")
        groups_list = groups[1].split(' ')
        for element in groups_list:
            if element == '.':
                temp.append(os.curdir+"/")
            else:
                temp.append(element)
        verboseprint("returning paths to caller")
        return temp
    verboseprint("done set_paths")
    return []


def set_alias(command):
    verboseprint("set_alias started")
    groups = regex_new_alias.match(command)
    if groups:
        verboseprint("match")
        aliases[groups[2]]=groups[3]
    else:
        verboseprint("what are my aliases? to make one it is set alias [name] \"command\"")
        for k,v  in aliases.items():
            print("{}: {}".format(k,v))
    verboseprint("done set_alias")

def remove_alias(command):
    verboseprint("remove_alias started")
    groups = regex_remove_alias.match(command)
    if groups and groups[2] in aliases:
        try:
            verboseprint("match")
            aliases.pop(groups[2])
        except Exception as e:
            print("ERROR! {}".format(e))
    else:
        print("ERROR! not in dictionary!")
    verboseprint("remove_alias ended")

'''
run_command
This is what the child process runs when the parent is forked
'''
def run_command(arg_items, paths):
    verboseprint("run_command started")
    i = 0
    verboseprint("going throug the switchboard of try catches")
    while True:
        try:
            verboseprint("is there a {} int {}".format(arg_items[0],paths[i]))
            os.execv(paths[i]+arg_items[0],arg_items)
            verboseprint("yup")
            break
        except IndexError:
            try:
                verboseprint("lets try without the recomend paths...")
                os.execv(arg_items[0],arg_items)
                verboseprint("yup")
            except Exception as e:
                print("ERROR! {}".format(e))
            break
        except PermissionError:
            verboseprint("nope")
            i += 1
            continue
        except FileNotFoundError:
            verboseprint("nope")
            i += 1
        except Exception as e:
            print("ERROR! {}".format(e))
            break
    verboseprint("run_command done and killing the child process...")
    os._exit(0)

'''
show last commands
shows the elements in a list that is given
'''
def show_last_comands(commands):
    commands_length = len(commands)
    #I reversed it to show the latest command first is the
    #first command the user sees
    for i  in range(0,commands_length, 1):
            print("{}: {}".format((i),commands[i]))

def verboseprint(text):
    global verbose
    if verbose:
        print(text)
'''
start_shell
what the main function uses. to run the shell
'''
def start_shell(precommands):
    verboseprint("starting the shell")
    verboseprint("setting th path and stuff")
    paths = []
    regex  =  re.compile('\!\d+')
    last_commands = []
    precommands_len = len(precommands)
    precommands_index = 0
    verboseprint("set up complete")
    verboseprint("starting shell engines!")
    while True:
        args = []
        if -1 < precommands_len > 0 and precommands_index < precommands_len:
            verboseprint("PRECOMAND: {}".format(precommands[precommands_index]))
            given_command = precommands[precommands_index]
            precommands_index+=1
        else:
            given_command = input("osh>")
            sys.stdout.flush()
        verboseprint("figguring out what to do with command")
        if given_command is None or given_command is '':
            continue
        elif given_command == '!!':
            show_last_comands(last_commands)
            continue
        elif given_command.startswith("alias"):
            set_alias(given_command)
            continue
        elif given_command.startswith("unalias"):
            remove_alias(given_command)
            continue
        elif given_command.startswith("set path = ("):
            paths = set_paths(given_command)
            print(paths)
            continue
        elif given_command == "path":
            print(paths)
            continue
        elif given_command.startswith("set verbose"):
            toggle_chaty(given_command)
            continue
        elif regex.match(given_command):
            verboseprint("older command")
            #need to get ring of the '!' the given command
            #can be converted into an int!
            index = (int) (given_command.replace("!",""))
            if index < len(last_commands):
                args = last_commands[index].split(' ')
            else:
                print("no such command in history!")
                continue
        else:
            verboseprint("preparing to execute command")
            if given_command in aliases:
                verboseprint("input was an alias")
                given_command = aliases[given_command]
            verboseprint("append last command")
            last_commands.append(given_command)
            verboseprint("seting args")
            args = last_commands[-1].split(' ')
        if args[0] != "exit":
            verboseprint("forking")
            pid = os.fork()
            if pid > 0:
                if "&" in args:
                    verboseprint("parent is waiting")
                    os.wait()
            if pid == 0:
                #if we leave the '&' in there
                #the program errors
                if "&" in args:
                    verboseprint("getting rid of the '&'")
                    run_command(args[0:-1],paths)
                else:
                    verboseprint("run")
                    run_command(args,paths)
        else:
            verboseprint("nothing")
            break
        if len(last_commands) > 11:
            verboseprint("getting rid of {}".format(last_commands.pop(0)))
'''
main
driver function
'''
def main():
    start_shell(with_pre_commands())
    verboseprint("bye")

main()
