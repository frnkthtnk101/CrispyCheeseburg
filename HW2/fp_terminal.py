'''
'FPTerminal.py
'Homework 2 part 1 and part 2 and extra assignments
'A concept shell
'Franco Pettigrosso
'''
import os #used to run fork, execvp
import re #make sure the user is calling ! command correctly
import sys #get special folders and stuff

#GLOBALS - I know people do not like using them. But they seem apporiate!
ALIASES = dict()
VERBOSE = False
#no point recompiling these evertime, easier on machine
REGEX_NEW_ALIAS = re.compile('(alias) (\w+) \"([\w -]+)\"')
REGEX_REMOVE_ALIAS = re.compile('(unalias) (\w+)')
REGEX_GRAB_PATHS = re.compile('set path = \((.+)\)')

'''
'toggle_chatty
'turns VERBOSE on and off
'''
def toggle_chaty(command):
    global VERBOSE
    if command == "set VERBOSE on":
        VERBOSE = True
        v_print("hello im chatty today..")
    else:
        v_print("goodbye!")
        VERBOSE = False

'''
'with_pre_commands
'identifies if the .cs543rc file is in the home directory
'if so writes down the commands to a list
'''
def with_pre_commands():
    v_print("with precommands started")
    path = "{}/.cs543rc".format(os.path.expanduser("~"))
    v_print("getting path")
    temp = []
    v_print("seeing if that .cs543rc file exist")
    if os.path.exists(path):
        v_print("getting commands")
        reader = open(path, "r")
        for line in reader:
            #want to check if VERBOSE is on so the user
            #can see everything
            if line.startswith("set VERBOSE"):
                toggle_chaty(line.strip())
            else:
                temp.append(line.strip())
        reader.close
    v_print("done with_pre_commands")
    return temp

'''
'set_paths
'used to set the searching paths for commands
'''
def set_paths(command):
    v_print("set path started")
    groups = REGEX_GRAB_PATHS.match(command)
    temp = []
    if groups:
        v_print("match!")
        groups_list = groups[1].split(' ')
        for element in groups_list:
            if element == '.':
                temp.append(os.curdir+"/")
            else:
                temp.append(element)
        v_print("returning paths to caller")
        return temp
    v_print("done set_paths")
    return []

'''
'set_alias
'creates an alias and if there is none
'to create. it just shows the current
'''
def set_alias(command):
    v_print("set_alias started")
    groups = REGEX_NEW_ALIAS.match(command)
    if groups:
        v_print("match")
        ALIASES[groups[2]] = groups[3]
    else:
        v_print("what are my ALIASES? to make one it \
            is set alias [name] \"command\"")
        for k, v  in ALIASES.items():
            print("{}: {}".format(k, v))
    v_print("done set_alias")

'''
'remove_alias
'removes an alias
'''
def remove_alias(command):
    v_print("remove_alias started")
    groups = REGEX_REMOVE_ALIAS.match(command)
    if groups and groups[2] in ALIASES:
        try:
            v_print("match")
            ALIASES.pop(groups[2])
        except Exception as e:
            print("ERROR! {}".format(e))
    else:
        print("ERROR! not in dictionary!")
    v_print("remove_alias ended")

'''
run_command
This is what the child process runs when the parent is forked
'''
def run_command(arg_items, paths):
    v_print("run_command started")
    i = 0
    v_print("going through the switchboard of try catches")
    '''
    'I did it this way because it seem the fast way to do it
    'I could of made it check the directories first for the command
    'then execute in the right directory too.
    'the while loop exits out on #4 or #7 or #1 or #2 or #3
    'a normal path examples
    '5 elements in path.. input was ls and /bin/ is the third element in path
    '1,5,1,5,1,5,1
    'no elements. direct path... input was /bin/ls
    '1,2,3
    'no element in path and not direct path... input was ls
    '1,2,3,4 usually errors out sayin no file in directory
    '2 elements in path and given bologna... bologna
    '1,5,1,5,1,2,3,4
    '''
    while True:
        try:#1
            v_print("is there a {} int {}".format(arg_items[0], paths[i]))
            os.execv(paths[i]+arg_items[0], arg_items)
            v_print("yup")
            break
        except IndexError:#2
            try:#3
                v_print("lets try without the recomend paths...")
                os.execv(arg_items[0], arg_items)
                v_print("yup")
            except Exception as e:#4
                print("ERROR! {}".format(e))
            break
        except PermissionError:#5
            v_print("nope")
            i += 1
            continue
        except FileNotFoundError:#6
            v_print("nope")
            i += 1
        except Exception as e:#7
            print("ERROR! {}".format(e))
            break
    v_print("run_command done and killing the child process...")
    os._exit(0)

'''
show last commands
shows the elements in a list that is given
'''
def show_last_comands(commands):
    commands_length = len(commands)
    #I reversed it to show the latest command first is the
    #first command the user sees
    for i  in range(0, commands_length, 1):
        print("{}: {}".format((i), commands[i]))

'''
'v_print
'used to print extra stuff to the screen
'''
def v_print(text):
    global VERBOSE
    if VERBOSE:
        print(text)

'''
start_shell
what the main function uses. to run the shell
'''
def start_shell(precommands):
    v_print("starting the shell")
    v_print("setting th path and stuff")
    paths = []
    regex = re.compile('\!\d+')
    last_commands = []
    precommands_len = len(precommands)
    precommands_index = 0
    v_print("set up complete")
    v_print("starting shell engines!")
    while True:
        args = []
        if -1 < precommands_len > 0 and precommands_index < precommands_len:
            v_print("PRECOMAND: {}".format(precommands[precommands_index]))
            given_command = precommands[precommands_index]
            precommands_index += 1
        else:
            given_command = input("osh>")
            sys.stdout.flush()
        v_print("figguring out what to do with command")
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
        elif given_command.startswith("set VERBOSE"):
            toggle_chaty(given_command)
            continue
        elif regex.match(given_command):
            v_print("older command")
            #need to get ring of the '!' the given command
            #can be converted into an int!
            index = (int)(given_command.replace("!", ""))
            if index < len(last_commands):
                args = last_commands[index].split(' ')
            else:
                print("no such command in history!")
                continue
        else:
            v_print("preparing to execute command")
            if given_command in ALIASES:
                v_print("input was an alias")
                given_command = ALIASES[given_command]
            v_print("append last command")
            last_commands.append(given_command)
            v_print("seting args")
            args = last_commands[-1].split(' ')
        if args[0] != "exit":
            v_print("forking")
            pid = os.fork()
            if pid > 0:
                if "&" in args:
                    v_print("parent is waiting")
                    os.wait()
            if pid == 0:
                #if we leave the '&' in there
                #the program errors
                if "&" in args:
                    v_print("getting rid of the '&'")
                    run_command(args[0:-1], paths)
                else:
                    v_print("run")
                    run_command(args, paths)
        else:
            v_print("nothing")
            break
        if len(last_commands) > 11:
            v_print("getting rid of {}".format(last_commands.pop(0)))
'''
main
driver function
'''
def main():
    start_shell(with_pre_commands())
    v_print("bye")

main()
