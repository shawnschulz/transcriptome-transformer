///auto_complete contains the methods and structures for making completions and serializing.
use std::fs::File;
use std::io::prelude::*;

struct Commands 
{
    data: [Vec<String>; 2],
}

impl Commands{
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
pub fn insert_command<'a>(&mut self, new_command: String) -> Result<&'static str, &'static str>
{
    //Check if the entered command is just whitespace or newlines
    if new_command.len() == 0 || new_command.chars().all(|x| x == ' ') || new_command.chars().all(|x: char| x == '\n') || new_command == ""
    { 
        return Err("Error: received either an empty string or only whitespace as a command")
    }
    for category_i in 0..self.data.len()
    {
        for i in 0..self.data[category_i].len() 
        {
            if new_command == self.data[category_i][i]
            {
                let repeat_command = self.data[category_i].remove(i);
                self.data[0].insert(0, repeat_command.to_string());
                return Ok("Command found in previous self.data, placing at front of self.data");
            }
        }
    }
    //Actually have to reborrow our self.data here
    self.data[1].insert(0, new_command);
    return Ok("Command not run before, placing at front of self.data used once before")
}

/// Write the commmands array to a string. This function overwrites data, may need to create a new one that
/// edits if file size creates i/o performance problems  
/// 
/// ## Examples
/// ```
/// 
/// ```
fn write(&self, file_path: &str) -> Result<&'static str, &'static str>
 {
    let mut file = File::create(file_path).expect("Unable to write file");
    for i in 0..self.data.len()
    {
        for j in self.data[i].len()
        {
            file.write_all(&self.data[i][j].as_bytes()).expect("Something went wrong writing the command to file")
        }
    }
    Ok("File should have written succesfully")
 }
}


#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn test_insert_new_command() {
        let mut test_commands = Commands {data: [vec![], vec!["git add -a -m 'Add README.md'".to_string()]]};
        assert!(test_commands.insert_command("git add -a -m '".to_string()).is_ok());
        assert_eq!([vec![], vec!["git add -a -m '".to_string(), "git add -a -m 'Add README.md'".to_string()]], test_commands.data);
    }
    #[test]
    fn test_insert_old_command() {
        let mut test_commands = Commands {data: [vec![], vec!["git add -a -m 'Add README.md'".to_string()]]};
        assert!(test_commands.insert_command("git add -a -m 'Add README.md'".to_string()).is_ok());
        assert_eq!([vec!["git add -a -m 'Add README.md'".to_string()], vec![]], test_commands.data);
    }
    #[test]
    fn test_write()
    {
        let mut test_commands = Commands {data: [vec![], vec!["git add -a -m 'Add README.md'".to_string()]]};
        let file_path: &str = "./test.txt";
        test_commands.write(file_path);
    }

}
