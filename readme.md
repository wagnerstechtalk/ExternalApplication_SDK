# External application for AtGames Legends platform (Dockerized)

This repository provides a Dockerized cross-compile toolchain for the AtGames Legends platform. It offers a simple and straightforward way to build your C/C++ projects for AtGames Legends platform using Docker and CMake.

## Features:

- Built upon the robustness of Ubuntu 16.04.
- Provides essential build tools like `gcc-aarch64-linux-gnu`, `g++-aarch64-linux-gnu`, and more.
- Simple run script to either invoke the toolchain or start an interactive shell.
- Automated SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer download, build, and installation.

## Structure:
- Dockerfile: Contains the instructions to build the Docker image with the necessary tools.
- run.sh: A Bash script to automate the Docker build (if required) and run processes.
- media/build.sh: A Bash script that automates the build process for a given project directory.
- 3rd-party/configure.sh: A Bash script that automates the build process for SDL2 libraries.

## Requirements:

- Docker

## Usage:

1. **Clone the repository**
git clone https://github.com/wagnerstechtalk/ExternalApplication_SDK.git
cd ExternalApplication_SDK

2. **Place your C/C++ project inside the media directory.**
You can use the provided project structures as templates. For instance:

- hello-world: A basic C++ example that prints text to the console. Note that on the AtGames platform, you won't see terminal console outputs.
- sdl2-example: A basic snake game using SDL2.

**Project Structure:**
your_project_name/
├── build/                 # CMake build directory
├── CMakeLists.txt         # CMake configuration file
├── src/                   # Your source files directory
└── USB/                   # Assets directory. Executables are copied here after build.
    ├── your_project_name.elf   # The executable
    ├── your_project_name.png   # Boxart for the executable
    └── your_project_name.xml   # XML containing the name of the application

Inside your_project_name.xml:
<root>
    <name>your_project_name</name>
</root>

The build process automatically copies the resulting executable to the USB directory.

3. **Run the toolchain script**

Open a Command Prompt/Terminal/Console window and run the following command:

If you're looking to build a specific project:
	- For WSL, linux or mac users:
		./run.sh project_directory_name_inside_media
	- For windows users:
		run.bat "project_directory_name_inside_media"

To start an interactive bash session inside the Docker environment:
	- For WSL, linux or mac users:
		./run.sh
	- For windows users:
		run.bat

## Deployment:

Once you've compiled your project using the provided toolchain, you'll need to deploy the built binaries to your AtGames Legends device for execution.

1. **Locate the Compiled Binaries:**
After successful compilation, the resulting binary and associated assets will be located inside the USB directory of your project.

2. **Copy to an External Drive:**
To run the compiled source on your target machine:
	- Connect an external FAT32 drive to your computer.
	- Create a folder named "external" on the root of this drive if it doesn't exist.
	- Create a subfolder inside the external folder with the same name as your project (e.g., your_project_name).
	- Copy the contents of your_project_name/USB to the external/your_project_name directory on your external drive.

3. **Insert the External Drive to Your Machine:**
Once you've copied over the binaries and assets, safely eject the external drive from your computer and connect it to your AtGames Legends device. Your application should now be ready for execution.

## Step by step guide for a simple example to make the build for a snake game:

1. **Clone the repository**
Clone the git repository or download the zip file directly from the github page.
To clone the repository, follow the steps below.
	- Open the command prompt on your computer
	- Copy the command `git clone https://github.com/your_github_username/your_repository_name.git` and paste it into the command prompt, then press enter
	- Enter this command to your command prompt `cd your_repository_name` to change directory into the cloned repository.

2. **Build the snake game**
You can build the snake game with the provided tools.
	- If you are in the correct directory (Last step of the "Clone the repository"), then
	- For windows users:
		- Paste this command to the command prompt: `run.bat "sdl2-example"`
	- For WSL, linux or mac users:
		- Paste this command to the terminal: `./run.sh sdl2-example`
	- For the first build, it might take a little bit more time.
	- If the build was succesful you will see this directory structure for the "USB" directory:
		media
		└── sdl2-example
			└── USB/                   # Assets directory. Executables are copied here after build.
				├── res/               # The resources of the game
				├── sdl2-example.elf   # The executable
				├── sdl2-example.png   # Boxart for the executable
				└── sdl2-example.xml   # XML containing the name of the application

3. **Deploying the game**
To run the "sdl2-example" on your target machine:
	- Connect an external FAT32 drive to your computer.
	- Create a folder named `external` on the root of this drive if it doesn't exist.
	- Create a subfolder inside the external folder with the `sdl2-example` (e.g., your_project_name).
	- Copy the contents of `media/sdl2-example/USB` to the `sdl2-example` directory on your external drive.
	- Once you've copied over the binaries and assets, safely eject the external drive from your computer and connect it to your AtGames Legends device. Your application should now be ready for execution.


## Third-Party Libraries:

The Docker initialization script (3rd-party/configure.sh) handles the download, build, and installation of the SDL2 libraries. These libraries include:

- SDL2
- SDL2_image (with webp and tif disabled)
- SDL2_ttf (with freetype and harfbuzz built-in)
- SDL2_mixer

Source files are downloaded from their respective official SDL repositories, then built and installed into the container. This ensures that you have the necessary dependencies ready for your applications.

## Contributing:

- Pull requests are welcome! Please ensure your modifications do not break existing functionality. For major changes, please open an issue first to discuss the proposed change.

## License:

