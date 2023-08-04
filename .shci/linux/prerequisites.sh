function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt
