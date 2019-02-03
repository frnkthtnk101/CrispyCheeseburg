# redirect sys.stdout to a buffer
import sys, io
stdout = sys.stdout
sys.stdout = io.StringIO()

# call module that calls print()
print("hey")

# get output and restore sys.stdout
output = sys.stdout.getvalue()
sys.stdout = stdout

print(output)




'''
start_shell
what the main function uses. to run the shell
'''
def start_shell(precommands):
    paths = ["/bin/","/DOOM/"]
    alias_command = "alias"
    while True:
        #some other code..
        given_command = input("osh>")
        #some other code..
        elif given_command.startswith(alias_command):
            set_alias(given_command)
            continue
        elif given_command.startswith("unalias"):
            remove_alias(given_command)
            continue
        elif given_command.startswith("set path = ("):
            paths = set_paths(given_command)
            #we check here to make sure it ok ---v
            alias_command = set_alias_if_needed_to_be_change(alias_command, paths)
            print(paths)
            continue
        #some other code....

def set_alias_if_needed_to_be_change(command, paths):
    for i in paths:
        if os.path.exists(path):
            print("WARNING: you gave me a path with the {} command in it.\
                 The new name for alias is (generic alias)".format(command))
            return "generic alias"
    return "alias"