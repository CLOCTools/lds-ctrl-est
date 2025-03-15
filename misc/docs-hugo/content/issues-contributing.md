# Reporting Issues
If you encounter bugs when using this library or have specific feature requests that you believe fall within the stated scope of this project, please [open an issue on GitHub](https://github.com/stanley-rozell/lds-ctrl-est/issues) and use an appropriate issue template where possible. You may also fork the repository and submit pull-requests with your suggested changes.

# Contributing
We welcome any community contributions to this project. Please fork the repository and if possible use `clang-format` and `clang-tidy` to conform to the coding format/style of this repository.

When editing any documentation/guides, please use the markdown docs in `misc/docs-hugo` instead of directly editing the HTML docs. 
This may require having `hugo`, `graphviz` and `doxygen` installed through `brew`, as well as `doxybook2` installed through a git clone.

Clone the `doxybook2` repository online and place the executable in your `usr/local/bin` folder or another `bin` folder. 
Run `sudo chmod +x /usr/local/bin/doxybook2` to give doxybook2 permissions. Run `doxybook2 --help` to ensure that this works properly. If permission is still denied, navigate to System Preferences > Security & Privacy > General.
You should see a message at the bottom that says: "doxybook2 was blocked from use because it is not from an identified developer." Click Allow Anyway.

Updated docs can be built by running `cd scripts` and `./update-docs.sh`. After a successful build, navigate to `/misc/docs-hugo/` and run `hugo server` to create a local host of the website. Access the website using local host to ensure the correct results.
