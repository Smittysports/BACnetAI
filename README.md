<h1 style="color:lightblue;">Git</h1>

# Prerequisites
<u>**Windows and Linux**</u>
Create a GitHub account on https://github.com/
- If you already have one, skip this step
- This is mandatory for adding code changes to our repositories on GitHub
- Your ssh public keys will be in your account, these are needed to push your code

<u>**Windows only**</u>
1) Install the 'git for Windows' command prompt from https://git-scm.com/downloads
- This is needed to obtain the JavaSandbox code from the GitHub repository
- It also contain ssh, which will be very useful
- I recommend opening the command prompt after installing it, pinning it to the Windows taskbar, and then setting the 'Start in' directory to the location of your choice

<u>**Linux only**</u>
1) Install git with <br>
   sudo apt install git

# Usage
- Type the following from the command line to clone the repository
  ```bash
  git clone git@github.com:FRC-1721/JavaSandbox.git
- Check the type of remote communication used for the repository. Useful if you are trying to push code and your repository was accidentally cloned as HTTPS.
  ```bash
  git remote -v
- Change the type of communication manually or change your local code to point to a different repository. 
  - Example 1... I cloned the repository using HTTPS and then could not do a push, since I need to use SSH. 
  - Example 2... I created the JavaSandbox repository in my own personal GitHub account and then created a new one in the FRC-1721 GitHub; I then used this command to reassociate my code so  I could push it.
  ```bash
  git remote set-url origin git@github.com:FRC-1721/JavaSandbox.git
- Type the following to create your own branch
  ```bash
  git checkout -b YourBranchName
- Type the following to rename your own branch (useful if you came up with new ideas since creation)
  ```bash
  git branch -m YourBranchName YourNewBranchName
- Type the following to add changes that will will be used in a new commit that you would like added to the codebase
  ```bash
  git add .
- Type the following to commit your changes
  ```bash
  git commit -m "Add your comment"
- Type the following to push your changes to GitHub for merging into the repository
  ```bash
  git push -u origin YourBranchName
- Go to the link that is shown and prepare to create a draft or official pull request <br>
  - Choose the branch you want to merge to, usually main
  - Add thorough notes and a title
  - Click 'Create pull request' or 'Create Draft request'

<u>**Initial Setup of git**</u>
- Access: You will need to be authorized for the desired repositories on github.com/FRC-1721 in order to add anything. See the Software lead and/or mentors for push access.
- If you are using your own laptop or have your own login on a VM on the Proxmox server, you should tell git your user information by:
  1) Typing the following and replace "Your Name" with your actual name, make sure to use the double quotes
  ```bash
  git config --global user.name "FirstName LastName"
  ```
  2) Typing the following and replace "your_email@example.com" with the email you use with GitHub, make sure to use the double quotes
  ```bash
  git config --global user.email "your_email@example.com"
  ```
- If you are using a shared laptop, you should tell git your user information by:
  1) Going to the directory that contains your repository
  2) Typing the following and replace "Your Name" with your actual name, make sure to use the double quotes
  ```bash
  git config user.name "FirstName LastName"
  ```
  3) Typing the following and replace "your_email@example.com" with the email you use with GitHub, make sure to use the double quotes
  ```bash
  git config user.email "your_email@example.com"
  ```
- Create ssh keys <br>
  1) These are mandatory for pushing your code to GitHub
  2) Run this command to generate a new SSH key
  ```bash
  ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
  ```
     - Replace "your_email@example.com" with the email you use for GitHub
     - Accept the default location to save the key
     - You do not need to use a passphrase, but it is always recommended. If you forget it, you will have to go through this process again
  3) Start the ssh agent
  ```bash
  eval "$(ssh-agent -s)"
  ```
  4) Add your SSH private key to the agent
  ```bash
  ssh-add ~/.ssh/id_rsa
  ```
  5) Share your ssh public key with GitHub by:
  ```bash
  cat ~/.ssh/id_rsa.pub
  ```
     - This will display your public SSH key. The key should start with ssh-rsa and end with your email address.
     - Copy the entire key (including ssh-rsa at the beginning and your email at the end). <br>
  a) Log in to your personal GitHub account, go to 'settings', go to 'SSH and GPG keys' <br>
  b) Click the 'New SSH key' button <br>
  c) Paste your public key into the 'Key field' and give it a title <br>
  d) Click 'Add SSH key' <br>
  e) Test if it works with:
  ```bash
  ssh -T git@github.com
  ```
- Use SSH over a different port
  - The firewall blocks the default SSH port (22), but GitHub supports SSH over port 443, which is usually used for HTTPS
  - Edit the ~/.ssh/config file and add or change it to the following <br>
  ```bash
  Host github.com
    Hostname ssh.github.com
    Port 443
  ```
  - Test the connection with:
  ```bash
  ssh -T git@github.com
  ```
