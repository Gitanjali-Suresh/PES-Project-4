## PES Project 4 ReadMe <br />
# **Team members**<br /> 
Gitanjali Suresh (gisu9983) and Sarayu Managoli (sama2321)<br /><br />
# **Description**<br />
This repository contains custom Makefile along with the source codes for the implementation of Project 4 for the course Principles of Embedded Software. The repository also includes the output captures from Logic Analyzer for I2C operations. 
The Makefile consists of mainly three build targets, `Normal`, `Debug_LED` and `Test` along with the `clean` target.<br /><br />
# **Installation/Execution notes**<br />
1.	Open `MCUXpresso IDE`
2.	Create a new project
3.	Select `Files` > `New` > `Project` > `MCUXpresso IDE` > `New C/C++ Project` 
4.	Select `frdmkl25z` board
5.	Click on `Next` and provide `project suffix` name
6.	Click on `Next`
7.	Select `Redirect SDK “PRINTF” to C library “printf”`
8.	Click on `Finish`
9.	In order to use our own Makefile, we must uncheck the option to auto-generate it
10.	To do the above, click on the Project `settings` on your left
11.	Right click on `Option` > `Edit Options` > `C/C++ Builds` > Uncheck `Generate Makefiles automatically`
12.	 Create your own empty `debug` folder<br /><br />

# **Build using targets**<br />
1.	Right click on the project created
2.	Select `Build Target`
3.	Click on `create`
4.	Provide different build targets. Here, targets `all VERSION = Normal`,`all VERSION = Debug_LED`, ,`all VERSION = Test` and `clean` are created
5.	Only files under these targets will be built<br /><br />
# **Difficulties faced**<br />
1.	Working with the temperature sensor TMP102 with the correct device addressing<br />
2.	Understanding uC Unit Tests and writing the same for our piece of code.<br />
3. 	Generation of Alert signal from TMP102<br /><br />
Reading a few references and documentations for the above difficulties helped us in solving the difficulties.<br /><br />

# **Documentation for output**<br />
The outputs from the logic analyzer are captured in a separate PDF to show the different operations performed in an I2C protocol such as read, write, start and stop<br /><br />



