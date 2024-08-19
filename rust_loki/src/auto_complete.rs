///auto_complete contains the methods and structures for making completions and serializing.


///Checks where if a string representing a run command exists in the commands list, then inserts at the front if it does. If it doesn't it inserts it
/// in the back category
/// 
/// ## Examples 
/// 
/// ```
///let mut test_commands: [Vec<String>; 2]= [vec![], vec!["git add -a -m 'Add README.md'".to_string()]];
///assert!(insert_command("git add -a -m '".to_string(), &mut test_commands).is_ok());
///assert_eq!([vec![], vec!["git add -a -m '".to_string(), "git add -a -m 'Add README.md'".to_string()]], test_commands);
///
///let mut test_commands: [Vec<String>; 2]= [vec![], vec!["git add -a -m 'Add README.md'".to_string()]];
///assert!(insert_command("git add -a -m 'Add README.md'".to_string(), &mut test_commands).is_ok());
///assert_eq!([vec!["git add -a -m 'Add README.md'".to_string()], vec![]], test_commands);
/// ```
/// 
/// ## Errors
/// 
/// insert_command will error if receives empty string, only white space or only new lines as a command
pub fn insert_command<'a>(new_command: String, commands: &'a mut [Vec<String>; 2]) -> Result<&'static str, &'static str>
{
    //Check if the entered command is just whitespace or newlines
    if new_command.len() == 0 || new_command.chars().all(|x| x == ' ') || new_command.chars().all(|x: char| x == '\n') || new_command == ""
    { 
        return Err("Error: received either an empty string or only whitespace as a command")
    }
    for category_i in 0..commands.len()
    {
        for i in 0..commands[category_i].len() 
        {
            if new_command == commands[category_i][i]
            {
                let repeat_command = commands[category_i].remove(i);
                commands[0].insert(0, repeat_command.to_string());
                return Ok("Command found in previous commands, placing at front of commands");
            }
        }
    }
    //Actually have to reborrow our commands here
    commands[1].insert(0, new_command);
    return Ok("Command not run before, placing at front of commands used once before")
}

// pub fn write_to_file(commands:Vec<String>) -> Result<&'static str, &'static str>
// {

// }

#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn test_insert_new_command() {
        let mut test_commands: [Vec<String>; 2]= [vec![], vec!["git add -a -m 'Add README.md'".to_string()]];
        assert!(insert_command("git add -a -m '".to_string(), &mut test_commands).is_ok());
        assert_eq!([vec![], vec!["git add -a -m '".to_string(), "git add -a -m 'Add README.md'".to_string()]], test_commands);
    }
    #[test]
    fn test_insert_old_command() {
        let mut test_commands: [Vec<String>; 2]= [vec![], vec!["git add -a -m 'Add README.md'".to_string()]];
        assert!(insert_command("git add -a -m 'Add README.md'".to_string(), &mut test_commands).is_ok());
        assert_eq!([vec!["git add -a -m 'Add README.md'".to_string()], vec![]], test_commands);
    }

}
