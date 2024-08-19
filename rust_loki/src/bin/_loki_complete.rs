use std::{
    fs::File,
    io::{prelude::*, BufReader},
    path::Path,
};
// //TODO: generate subcommands from a file, write commands you run through lo in shell to that file
// //two levels of subdivision, the subcommands and the commands

// ///Outlines all of the types of commands possible that are flagged in compgen
// enum CommandType {
//     arg,
//     directory,
//     host_name,
// }

// ///A command, which contains a command name and its subcommands/args
// struct LoCommand<'a> {
//     command_name: String,
//     sub_commands: Vec<&str>,
//     arg_type: CommandType,
// }

// ///A list of commands, which contains methods to produce lists of commands
// struct CommandsList {
//     commands: Vec<LoCommand>,
// }

// ///Make a CommandsList Struct from reading a file
// fn commands_from_file(file_path: &str) -> Vec<String> {
//     let file = File::open(file_path)?;
//     let mut s = String::new();
//     file.read_to_string(&mut s)?;
//     let result: Vec<String> = file.read_to_string(&mut s).lines().collect();
//     return result;

// }

// ///Write LoCommands to a file
// fn write_lo_to_file(commands: LoCommand) {
// }

// ///Our bash executable calls

// ///Generate the commands list using a bash executable
// fn generate_commands_list(file_path: String) {
// }

// ///Run complete -C on the compiled executable, this is gonna need to be moved elsewhere but can sit here for now
// fn run_complete() {

// }

fn main() {
    // let commands: Vec<String> = commands_from_file("./commands.txt");
    // //This lets us get the sub command being completed, but we need a 
    // //further subdivision by command as a whole
    // let args : Vec<String> = std::env::args().collect();                                                           
    // let word_being_completed = &args[2];                                                                           

    // //The order of the commands in commands.txt matters, should create function that reorders from buffer based on overlapping strings
    // for command in commands {
    //     if command.starts_with(word_being_completed) {
    //         println!("{}", command);
    //     }
    // }
}
