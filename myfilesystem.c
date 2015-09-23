#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <libgen.h>

struct NODE *root, *cwd;
char dname[64], bname[64];

const char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "quit", "help", "?", "menu", "reload", "save", 0};

const char *description[] = {"Creates new directory\nmkdir <pathname>\n",
					   "Removes existing directory\nrmdir <pathname>\n",
					   "Displays contents of given directory, current working\ndirectory if none specified\nls <pathname>\n",
					   "Changes current working directory to given directory,\nto root directory if none specified\ncd <pathname>\n",
					   "Displays path of current roking directory\n",
					   "Creates new file\ncreat <pathname>\n",
					   "Removes existing file\nrm <pathname>\n",
				       "Purges filetree and ends program\n",
					   "Displays description and usage for a given command\nhelp <command>\n",
					   "Displays description and usage for a given command\n? <command>\n",
					   "Displays all acceptable commands\n",
					   "Loads previously saved filetree from a given .txt file\nreload <file>\n",
					   "Saves current filetree to a given .txt file\nsave <file>\n",
					   0};

typedef struct NODE
{
    char *name;
    char type;
    struct NODE *childPtr, *siblingPtr, *parentPtr;
} NODE;

int main()
{
    initialize();

	int ID;
    while(1)
    {
        printf("input a command: "); // Prompt for input

		char line[128], command[32], pathname[64];
		memset(line, 0, 128);
		memset(command, 0, 32);
		memset(pathname, 0, 64);

        fgets(line, 128, stdin); // Read in line
        line[strlen(line)] = 0;

        sscanf(line, "%s %s", command, pathname);

        ID = find_command(command); // figure out command code

        switch(ID)
		{
            case 0 : mkdir(pathname);     break;
            case 1 : rmdir(pathname);    break;
            case 2 : ls(pathname);    break;
            case 3 : cd(pathname);       break;
            case 4 : pwd(pathname);       break;
            case 5 : creat(pathname);     break;
            case 6 : rm(pathname);    break;
			case 7 : quit();       break;
			case 8 : help(pathname);       break;
            case 9 : help(pathname);    break;
            case 10 : menu();       break;
            case 11 : reload(pathname);    break;
            case 12 : save(pathname);       break;
        }
    }
}

int initialize()
{
    root = (NODE*) malloc(sizeof(NODE));
    root->name = "/";
    root->type = 'D';
    root->childPtr = NULL;
    root->parentPtr = NULL;
    root->siblingPtr = NULL;

    cwd = root;
}

struct NODE * search(char *path, struct NODE *current)
{
	if (path[0] == '/') // Absolute
    {
		current = root;

		if (strcmp(path, current->name) == 0 || strcmp(path, ".") == 0) // if path is root
			return current;

		char *next = strtok(path, "/"); // First token

		int exists;

		if (strcmp(next, ".") != 0)
			return root->childPtr;

		exists = 0;
		if (current->childPtr != NULL) // if root has children
		{
			if (strcmp(current->childPtr->name, next) == 0) // Check childPtr
			{
				current = current->childPtr;
				exists = 1;
			}
			else
			{
				struct NODE *sibling = current->childPtr;
				while (sibling->siblingPtr != NULL) // While sibling exists
				{
					if (strcmp(sibling->siblingPtr->name, next) == 0) // check sibling
					{
						current = sibling->siblingPtr;
						if (current->type == 'F') // if not dir
					    {
					        printf("%s is a file, not a directory\n", next); // Path is a FILE and not a DIR
					        return;
					    }
						exists = 1;
						break;
					}
					sibling = sibling->siblingPtr;
				}
			}
		    if (exists == 0) // Dir not found
			{
			    printf("%s not found: No such file or directory\n", next);
			    return;
			}
		}
		else
		{
			printf("%s not found: No such file or directory\n", next);
			return;
		}

		while (next = strtok(0, "/") && strcmp(next, ".") != 0) // Parse the rest of the path
		{
			exists = 0;
			if (current->childPtr != NULL) // if root has children
			{
				if (strcmp(current->childPtr->name, next) == 0) // Check childPtr
				{
					current = current->childPtr;
					exists = 1;
				}
				else
				{
					struct NODE *sibling = current->childPtr;
					while (sibling->siblingPtr != NULL) // While sibling exists
					{
						if (strcmp(sibling->siblingPtr->name, next) == 0) // check sibling
						{
							current = sibling->siblingPtr;
							if (current->type == 'F') // if not dir
						    {
						        printf("%s is a file, not a directory\n", next); // Path is a FILE and not a DIR
						        return;
						    }
							exists = 1;
							break;
						}
						sibling = sibling->siblingPtr;
					}
				}
				if (exists == 0) // Dir not found
				{
					printf("%s not found: No such file or directory\n", next);
					return;
				}
			}
			else
			{
				printf("%s not found: No such file or directory\n", next);
				return;
			}
		}
	}
	else // Relative
	{
		current = cwd;

		if (strcmp(path, current->name) == 0) // if path is root
			return current;

		char *next = strtok(path, "/"); // First token

		if (strcmp(next, ".") != 0)
			return cwd->childPtr;

		int exists;

		exists = 0;
		if (current->childPtr != NULL) // if root has children
		{
			if (strcmp(current->childPtr->name, next) == 0) // Check childPtr
			{
				current = current->childPtr;
				exists = 1;
			}
			else
			{
				struct NODE *sibling = current->childPtr;
				while (sibling->siblingPtr != NULL) // While sibling exists
				{
					if (strcmp(sibling->siblingPtr->name, next) == 0) // check sibling
					{
						current = sibling->siblingPtr;
						if (current->type == 'F') // if not dir
					    {
					        printf("%s is a file, not a directory\n", next); // Path is a FILE and not a DIR
					        return;
					    }
						exists = 1;
						break;
					}
					sibling = sibling->siblingPtr;
				}
			}
		    if (exists == 0) // Dir not found
			{
			    printf("%s not found: No such file or directory\n", next);
			    return;
			}
		}
		else
		{
			printf("%s not found: No such file or directory\n", next);
			return;
		}

		while (next = strtok(0, "/") && strcmp(next, ".") != 0) // Parse the rest of the path
		{
			exists = 0;
			if (current->childPtr != NULL) // if root has children
			{
				if (strcmp(current->childPtr->name, next) == 0) // Check childPtr
				{
					current = current->childPtr;
					exists = 1;
				}
				else
				{
					struct NODE *sibling = current->childPtr;
					while (sibling->siblingPtr != NULL) // While sibling exists
					{
						if (strcmp(sibling->siblingPtr->name, next) == 0) // check sibling
						{
							current = sibling->siblingPtr;
							if (current->type == 'F') // if not dir
						    {
						        printf("%s is a file, not a directory\n", next); // Path is a FILE and not a DIR
						        return;
						    }
							exists = 1;
							break;
						}
						sibling = sibling->siblingPtr;
					}
				}
				if (exists == 0) // Dir not found
				{
					printf("%s not found: No such file or directory\n", next);
					return;
				}
			}
			else
			{
				printf("%s not found: No such file or directory\n", next);
				return;
			}
		}
	}	

	return current;
}

char * get_dirname(char *pathname)
{
    char temp[128];
    strcpy(temp, pathname);
    strcpy(dname, dirname(temp));   // dname="/a/b"

	return dname;
}

char * get_basename(char *pathname)
{
    char temp[128];
	strcpy(temp, pathname);
    strcpy(bname,basename(temp));   // bname="c"

	return bname;
}

int mkdir(char *pathname)
{
	if (pathname[0] == 0)
	{
		printf("mkdir: missing operand\n");
		return;
	}

	char *bname = get_basename(pathname);
	char *dname = get_dirname(pathname);

    struct NODE *current;
	current = search(dname, current);

	if (current->childPtr != NULL) // if current has children
	{
		int exists = 0;
		if (strcmp(current->childPtr->name, bname) == 0 && current->childPtr->type == 'D') // Check childPtr
		{
			printf("mkdir: cannot create directory '%s': File exists\n", bname);
			return;
		}
		else
		{
			struct NODE *sibling = current->childPtr;
			while (sibling->siblingPtr != NULL) // While sibling exists
			{
				if (strcmp(sibling->siblingPtr->name, bname) == 0 && sibling->siblingPtr->type == 'D') // check sibling
				{
					printf("mkdir: cannot create directory '%s': File exists\n", bname);
					return;
				}
				sibling = sibling->siblingPtr;
			}
			if (sibling->siblingPtr == NULL)
			{
				struct NODE *new_node = (NODE*) malloc(sizeof(NODE));
				new_node->name = bname;
				new_node->type = 'D';
				new_node->siblingPtr = NULL;
				new_node->parentPtr = current;
				new_node->childPtr = NULL;

				sibling->siblingPtr = new_node;
				return;
			}
		}
	}
	else
	{
		struct NODE *new_node = (NODE*) malloc(sizeof(NODE));
        new_node->name = bname;
        new_node->type = 'D';
        new_node->siblingPtr = NULL;
        new_node->parentPtr = current;
        new_node->childPtr = NULL;

		current->childPtr = new_node;
        return;
	}

	printf("%s couldn't be created\n", bname);
	return -1;
}

int rmdir(char *pathname)
{
	if (pathname[0] == 0)
	{
		printf("rmdir: missing operand\n");
		return;
	}

	char *bname = get_basename(pathname);
	char *dname = get_dirname(pathname);

    struct NODE *current;
	current = search(dname, current);

	if (current->childPtr != NULL) // if current has children
	{
		if (strcmp(current->childPtr->name, bname) == 0 && current->childPtr->type == 'D') // Check childPtr
		{
			current->childPtr = current->childPtr->siblingPtr;
			return;
		}
		else
		{
			struct NODE *sibling = current->childPtr;
			while (sibling->siblingPtr != NULL) // While sibling exists
			{
				if (strcmp(sibling->siblingPtr->name, bname) == 0 && sibling->siblingPtr->type == 'D') // check sibling
				{
					sibling->siblingPtr = sibling->siblingPtr->siblingPtr;
				}
				sibling = sibling->siblingPtr;
			}
		}
	}
	else
	{
		printf("rmdir: failed to remove '%s': No such file or directory\n", bname);
		return;
	}

    // if not found
	printf("rmdir: failed to remove '%s': No such file or directory\n", bname);
	return -1;
}

int cd(char *pathname)
{
	if (pathname[0] == 0)
	{
		cwd = root;
		return;
	}

   	char *bname = get_basename(pathname);
	char *dname = get_dirname(pathname);

    struct NODE *current;
	current = search(dname, current);

	cwd = current;
	return;
}

int ls(char *pathname)
{
	char *bname = get_basename(pathname);
	char *dname = get_dirname(pathname);

	struct NODE *current;

	if (pathname[0] == 0)
		current = cwd;
	else
		current = search(dname, current);

	if (current->childPtr != NULL) // if current has children
	{
		struct NODE *sibling = current->childPtr;
		while (sibling->siblingPtr != NULL) // While sibling exists
		{
			printf("%c %s\n", sibling->type, sibling->name);
			sibling = sibling->siblingPtr;
		}
	}
	else
	{
		printf("Directory is empty\n");
		return;
	}
	return;
}

int pwd()
{
    rpwd(cwd);
    printf("\n");
}

int rpwd(struct NODE *dir)
{
	if (dir == NULL)
    {
		return;
    }

    rpwd(dir->parentPtr);

	if (strcmp(dir->name, "/") == 0)
		printf("%s","/");
	else
    	printf("%s/",dir->name);
}

int creat(char *pathname)
{
	if (pathname[0] == 0)
	{
		printf("creat: missing operand\n");
		return;
	}
	char *bname = get_basename(pathname);
	char *dname = get_dirname(pathname);

    struct NODE *current;
	current = search(dname, current);

    int exists = 0;

	if (current->childPtr != NULL) // if current has children
	{
		exists = 0;
		if (strcmp(current->childPtr->name, bname) == 0 && current->childPtr->type == 'F') // Check childPtr
		{
			exists = 1;
		}
		else
		{
			exists = 0;
			struct NODE *sibling = current->childPtr;
			while (sibling->siblingPtr != NULL) // While sibling exists
			{
				if (strcmp(sibling->siblingPtr->name, bname) == 0 && sibling->siblingPtr->type == 'F') // check sibling
				{
					exists = 1;
					break;
				}

				if (sibling->siblingPtr == NULL)
				{
					struct NODE *new_node = (NODE*) malloc(sizeof(NODE));
					new_node->name = bname;
					new_node->type = 'F';
					new_node->siblingPtr = NULL;
					new_node->parentPtr = current;
					new_node->childPtr = NULL;

					sibling->siblingPtr = new_node;
					return;
				}
				sibling = sibling->siblingPtr;
			}
		}
		if (exists == 1) // Dir not found
		{
			printf("%s already exists\n", bname);
			return;
		}
	}
	else
	{
		struct NODE *new_node = (NODE*) malloc(sizeof(NODE));
        new_node->name = bname;
        new_node->type = 'F';
        new_node->siblingPtr = NULL;
        new_node->parentPtr = current;
        new_node->childPtr = NULL;

		current->childPtr = new_node;
        return;
	}

	printf("%s couldn't be created\n", bname);
	return -1;
}

int rm(char *pathname)
{
	if (pathname[0] == 0)
	{
		printf("rm: missing operand\n");
		return;
	}

	char *bname = get_basename(pathname);
	char *dname = get_dirname(pathname);

    struct NODE *current;
	current = search(dname, current);

    if (current->childPtr != NULL) // if current has children
	{
		if (strcmp(current->childPtr->name, bname) == 0 && current->childPtr->type == 'F') // Check childPtr
		{
			current->childPtr = current->childPtr->siblingPtr;
			return;
		}
		else
		{
			struct NODE *sibling = current->childPtr;
			while (sibling->siblingPtr != NULL) // While sibling exists
			{
				if (strcmp(sibling->siblingPtr->name, bname) == 0 && sibling->siblingPtr->type == 'F') // check sibling
				{
					sibling->siblingPtr = sibling->siblingPtr->siblingPtr;
				}
				sibling = sibling->siblingPtr;
			}
		}
	}
	else
	{
		printf("rm: failed to remove '%s': No such file or directory\n", bname);
		return;
	}

    // if not found
	printf("rm: failed to remove '%s': No such file or directory\n", bname);
	return -1;
}

int save(char *filename)
{
	FILE *f = fopen(filename, "w");
	rsave(f, root);
	fclose(f);
}

int rsave(FILE *f, struct NODE *dir)
{
	if (dir == NULL)
		return;

	fprintf(f, "%c %s\n", dir->type, dir->name);
	if (dir->childPtr != NULL)
	{
		rsave(f, dir->childPtr);
		
		struct NODE *sibling = dir->childPtr;
		while (sibling->siblingPtr != NULL)
		{
			rsave(f, sibling->siblingPtr);
			sibling = sibling->siblingPtr;
		}
	}
}

int reload(char *filename)
{
	FILE *f = fopen("myfiletree", "r");
	char *line, type, *name;
	fscanf(line, "%c %s", &type, name);
	if (strcmp(name, "/"))
	{
		initialize();
	}

	while (fscanf(line, "%c %s", &type, name) != EOF)
	{
		if (type == 'D')
			mkdir(name);
		else if (type == 'F')
			creat(name);
		else
			continue;
	}

	fclose(f);
}

int quit()
{
	printf("Quiting filesystem...\n");
	exit(0);
}

int find_command(char *command)
{
    int i = 0;
    while(cmd[i])
    {
        if (strcmp(command, cmd[i]) == 0)
            return i;
        i++;
    }
    return -1;
}

int help(char *command)
{
	if (command[0] == 0)
	{
		printf("help/?: missing operand\n");
		return;
	}

    int i = 0;
    while(cmd[i])
    {
        if (strcmp(command, cmd[i]) == 0)
		{
			printf("%s", description[i]);
            return;
		}
        i++;
    }
    return -1;
}

int menu()
{
    int i = 0;
    while(cmd[i])
    {
        printf("%s	", cmd[i]);
        i++;
    }
	printf("\n");
    return -1;
}
