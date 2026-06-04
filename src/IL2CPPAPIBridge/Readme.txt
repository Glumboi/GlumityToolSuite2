default_bridge_env contains 3 folders that need to be copied to the "plugins" folder of the game you installed Glumity2 to.
This will make jit possible by adding required libraries for basic functionality, and is expected to be there by this plugin, if not provided, crashes are likely!!!!

It also includes a quick example script, Glumity's internal functions as well as the IL2CPPDumper have not been made compatible with this yet, apart from the plugin printf function!

A single '.c' file represents a single JIT-plugin, this is a choice I made for easier file managing as well as just getting thins running.