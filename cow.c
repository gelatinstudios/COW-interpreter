
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {moo, mOo, moO, mOO, Moo, MOo, MoO, MOO, OOO, MMM, OOM, oom};

typedef struct {
        char *contents;
        size_t size;
} File;

inline File read_file(const char *filename) {
        File file = {0};

        FILE *f = fopen(filename, "rb");
        fseek(f, 0, SEEK_END);
        file.size = ftell(f);
        file.contents = malloc(file.size+1);
        fseek(f, 0, SEEK_SET);
        fread(file.contents, file.size, 1, f);
        fclose(f);

        file.contents[file.size] = 0;

        return file;
}

inline size_t parser(File file, unsigned char tokens[]) {
        size_t token_count = 0;
        char *token = strtok(file.contents, " \n\t");
        while(token) {
                const char *instructions[] = {"moo", "mOo", "moO", "mOO", "Moo", "MOo", "MoO", "MOO", "OOO", "MMM", "OOM", "oom"};
                unsigned char err = 1;
                for (size_t j = 0; j < 12; ++j) {
                        if (!strcmp(token, instructions[j])) {
                                tokens[token_count] = j;
                                ++token_count;
                                err = 0;
                                break;
                        }
                }
                if (err) {
                        fprintf(stderr, "unable to parse: %s\n", token);
                        free(file.contents);
                        exit(EXIT_FAILURE);
                }
                token = strtok(0, " \n\t");
        }


        return token_count;
}

inline void exec(size_t size, unsigned char tokens[size]) {
        unsigned char memory[30000] = {0};
        unsigned char reg;
        unsigned char reg_set = 0;
        size_t mem_pos = 0;
        size_t i = 0;
        while (i < size) {
                unsigned char instruction = tokens[i];
EXEC:           switch(instruction) {
                        case moo: do {--i;} while (tokens[i] != MOO); break;
                        case mOo: --mem_pos; break;
                        case moO: ++mem_pos; break;
                        case mOO: instruction = memory[mem_pos]; goto EXEC;
                        case Moo: if(!memory[mem_pos]) memory[mem_pos] = getchar();
                                  else putchar(memory[mem_pos]);
                                  break;
                        case MOo: --memory[mem_pos]; break;
                        case MoO: ++memory[mem_pos]; break;
                        case MOO: if(!memory[mem_pos]) {do {++i;} while (tokens[i] != moo);} break;
                        case OOO: memory[mem_pos] = 0; break;
                        case MMM: if(!reg_set) {
                                reg = memory[mem_pos];
                                reg_set = 1;
                        } else {
                                memory[mem_pos] = reg;
                                reg_set = 0;
                        } break;
                        case OOM: printf("%d", memory[mem_pos]); break;
                        case oom: scanf("%d", &memory[mem_pos]); break;
                }
                ++i;
        }
}

int main(int argc, const char *argv[argc + 1]) {
        if(argc < 2) {
                puts("no input file");
                return EXIT_FAILURE;
        }        
        File file = read_file(argv[1]);
        unsigned char tokens[file.size / 3];
        size_t size = parser(file, tokens);
        exec(size, tokens);
        free(file.contents);
}
