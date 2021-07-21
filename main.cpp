/*
 * Copyright (c) 2018, Yutaka Tsutano
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "command.hpp"
#include "parser.hpp"

#define NUM_COMMANDS_INTERACTIVE 25
#define READ_END 0
#define WRITE_END 1

int exec(const std::string& cmd, const std::vector<std::string>& args) {

    std::vector<char*> c_args = {const_cast<char*>(cmd.c_str())};
    for(const auto& a : args) {
        c_args.push_back(const_cast<char*>(a.c_str()));
    }
    c_args.push_back(nullptr);

    return execvp(cmd.c_str(), c_args.data()); 
}

void run_commands(std::vector<shell_command> shell_commands){
    pid_t pid;
    int status;
    int initial_read_end = 0;
    int fd;
    int fdp[2];

    for(std::vector<shell_command>::iterator cmd = shell_commands.begin();
        cmd != shell_commands.end(); ++cmd) {

        pipe(fdp);
        pid = fork();

        if(pid < 0) {
            std::cerr << "Fork Failed";
            exit(1);
        } else if(pid == 0) {

            if((cmd->cin_mode == istream_mode::pipe) && (cmd->cout_mode == ostream_mode::pipe)){
                close(fdp[READ_END]);
                dup2(initial_read_end, STDIN_FILENO);
                close(initial_read_end);

                dup2(fdp[WRITE_END], STDOUT_FILENO);
                close(fdp[WRITE_END]);

                if(cmd->cin_mode == istream_mode::file){
                    const char* inputFile = cmd->cin_file.c_str();
                    fd = open(inputFile, O_RDONLY, 0644);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::file){
                    const char* outputFile = cmd->cout_file.c_str();
                    fd = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::append){
                    const char* outputFileAppended = cmd->cout_file.c_str();
                    fd = open(outputFileAppended, O_CREAT | O_WRONLY | O_APPEND, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                exec(cmd->cmd, cmd->args);
                std::cerr << "Exec Failed";
                exit(1);

            } else if(cmd->cin_mode == istream_mode::pipe){
                close(fdp[WRITE_END]);
                dup2(initial_read_end, STDIN_FILENO);
                close(initial_read_end);

                if(cmd->cin_mode == istream_mode::file){
                    const char* inputFile = cmd->cin_file.c_str();
                    fd = open(inputFile, O_RDONLY, 0644);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::file){
                    const char* outputFile = cmd->cout_file.c_str();
                    fd = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::append){
                    const char* outputFileAppended = cmd->cout_file.c_str();
                    fd = open(outputFileAppended, O_CREAT | O_WRONLY | O_APPEND, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                exec(cmd->cmd, cmd->args);
                std::cerr << "Exec Failed";
                exit(1);

            } else if(cmd->cout_mode == ostream_mode::pipe){
                close(fdp[READ_END]);
                dup2(fdp[WRITE_END], STDOUT_FILENO);
                close(fdp[WRITE_END]);

                if(cmd->cin_mode == istream_mode::file){
                    const char* inputFile = cmd->cin_file.c_str();
                    fd = open(inputFile, O_RDONLY, 0644);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::file){
                    const char* outputFile = cmd->cout_file.c_str();
                    fd = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::append){
                    const char* outputFileAppended = cmd->cout_file.c_str();
                    fd = open(outputFileAppended, O_CREAT | O_WRONLY | O_APPEND, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                exec(cmd->cmd, cmd->args);
                std::cerr << "Exec Failed";
                exit(1);

            } else {
                if(cmd->cin_mode == istream_mode::file){
                    const char* inputFile = cmd->cin_file.c_str();
                    fd = open(inputFile, O_RDONLY, 0644);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::file){
                    const char* outputFile = cmd->cout_file.c_str();
                    fd = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                if(cmd->cout_mode == ostream_mode::append){
                    const char* outputFileAppended = cmd->cout_file.c_str();
                    fd = open(outputFileAppended, O_CREAT | O_WRONLY | O_APPEND, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                exec(cmd->cmd, cmd->args);
                std::cerr << "Exec Failed";
                exit(1);
            }
            
        } else {

            wait(&status);
            if(WEXITSTATUS(status) == 0){
                if((cmd->next_mode == next_command_mode::always) || (cmd->next_mode == next_command_mode::on_success)){
                    close(fdp[WRITE_END]);
                    initial_read_end = fdp[READ_END];
                    continue;
                } else if(cmd->next_mode == next_command_mode::on_fail) {
                    break;
                } else {
                    close(fdp[WRITE_END]);
                    initial_read_end = fdp[READ_END];
                    continue;
                }
            } else {
                if((cmd->next_mode == next_command_mode::on_fail) || (cmd->next_mode == next_command_mode::always)){
                    close(fdp[WRITE_END]);
                    initial_read_end = fdp[READ_END];
                    continue;
                } else {
                    break;
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::string input_line;
    std::vector<shell_command> shell_commands;

    if(argc > 1 && argv[1] == std::string("-t")) {
        
        while(std::getline(std::cin, input_line)){
            
            if(input_line == "exit"){
                exit(0);
            } else {
                try{
                    shell_commands = parse_command_string(input_line);
                    run_commands(shell_commands);

                    
                }
                catch(const std::runtime_error& e){
                    std::cout << e.what() << std::endl;
                }
            }
        }
    } else {
        for (int should_run = 0; should_run < NUM_COMMANDS_INTERACTIVE; should_run++) {
            // Print the prompt.
            std::cout << "osh> " << std::flush;

            // Read a single line.
            if (!std::getline(std::cin, input_line) || input_line == "exit") {
                break;
            }

            try {
                // Parse the input line.
                shell_commands = parse_command_string(input_line);
                run_commands(shell_commands);
            }
            catch (const std::runtime_error& e) {
                std::cout << e.what() << "\n";
            }
        }
    }

    std::cout << std::endl;
}