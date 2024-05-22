#!/bin/bash

# Start the lab environment
lab start files-review

# SSH into the remote server
ssh student@serverb << 'EOF'

# Create the directory and files for project plans
mkdir -p ~/Documents/project_plans
touch ~/Documents/project_plans/{season1,season2}_project_plans.odf

# List the directory contents recursively
ls -lR ~/Documents/

# Create audio files for TV seasons
touch ~/tv_season{1..2}_episode{1..6}.ogg
ls ~/tv*

# Create document files for a mystery book
touch ~/mystery_chapter{1..8}.odf
ls ~/mys*

# Create directories for video files and move files into them
mkdir -p ~/Videos/season{1..2}
ls ~/Videos
mv ~/tv_season1* ~/Videos/season1
mv ~/tv_season2* ~/Videos/season2
ls -R ~/Videos

# Create directories for a bestseller project
mkdir -p ~/Documents/my_bestseller/chapters
ls -R ~/Documents
mkdir ~/Documents/my_bestseller/{editor,changes,vacation}
ls -R ~/Documents

# Move into the chapters directory and organize files
cd ~/Documents/my_bestseller/chapters
mv ~/mystery_chapter* .
ls
mv mystery_chapter{1..2}.odf ../editor
ls
ls ../editor
mv mystery_chapter{7,8}.odf ../vacation
ls
ls ../vacation

# Move episode files into the vacation directory
cd ~/Videos/season2
cp *episode1.ogg ~/Documents/my_bestseller/vacation
ls
cd -
cp *episode2.ogg ~/Documents/my_bestseller/vacation
cd -
ls
ls ~/Documents/my_bestseller

# Copy specific chapters into changes directory and create backup files
cp chapters/mystery_chapter{5,6}.odf changes
ls chapters
ls changes
cp changes/mystery_chapter5.odf changes/mystery_chapter5_$(date +%F).odf
cp changes/mystery_chapter5.odf changes/mystery_chapter5_$(date +%s).odf
ls changes

# Remove the vacation directory and create backups directory
rm -r vacation
ls
cd
mkdir ~/Documents/backups

# Create a backup of the season2 project plan
ln ~/Documents/project_plans/season2_project_plans.odf ~/Documents/backups/season2_project_plans.odf.back
ls -lR ~/Documents

# Exit the remote session
exit
EOF

# Grade and finish the lab
lab grade files-review
lab finish files-review
