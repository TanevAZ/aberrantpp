#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIRECTIVE_MAX 64

int line_c = 0;
FILE *script_fp;

void skip_to(const char end)
{
    while (fgetc(script_fp) != end && !feof(script_fp)) {
        continue;
    }

    line_c++;
}

void append(char *str, char c) {
    int len = strlen(str);

    str[len] = c;
    str[len + 1] = '\0';
}

void get_to(const char end, char *ptr_buffer)
{
    char cur;

    while ((cur = fgetc(script_fp)) != end && !feof(script_fp)) {
        append(ptr_buffer, cur);
    }
}

int get_format(char *formatted)
{
    char cur;

    while ((cur = fgetc(script_fp)) != '"' && !feof(script_fp)) {
        continue;
    }

    // To do: implémenter les opérateurs avec les variables ex.: concaténations... (actuellement on ignore)

    if (cur == '"') {
        formatted[0] = '\0';

        get_to('"', formatted);
        skip_to(')');
    } else {
        printf("Erreur Aberrante: Il manque des \" quelque part\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <script>", argv[0]);
        return -1;
    }

    script_fp = fopen(argv[1], "r+b");

    char cur;

    
    while (!feof(script_fp)) {
        
        cur = fgetc(script_fp);

        switch (cur) {
            
            case '/': // Commentaire, ignorer la ligne complète 
                if ((cur = fgetc(script_fp)) == '/') {
                    skip_to('\n');
                }
                break;
            
            case '\n': // Compter les lignes
                line_c++;
                break;
            
            case ' ': // Ignorer les espaces
                continue;
            
            default:

                if ('a' < cur && cur < 'z') {

                    char directive[DIRECTIVE_MAX];
                    strcpy(directive, (char []){ cur, '\0' });

                    char format[64];
                    get_to('(', directive);

                    if (strcmp(directive, "cout") == 0) {
                        get_format(format);

                        write(STDOUT_FILENO, format, strlen(format));
                    } else {
                        printf("Erreur Aberrante: Retourne lire la doc, ta fonction '%s' n'existe pas\n", directive);
                    }
                }

                break;
        }
    }

    fclose(script_fp);
    return 0;
}
