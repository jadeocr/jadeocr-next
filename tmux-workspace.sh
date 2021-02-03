# !/bin/sh

tmux new -s jade -d

# working window
tmux send-keys 'cd client && vim' C-m

# client dev window
tmux split-window -v -t jade
tmux resize-pane -D 10
tmux send-keys 'cd client && yarn serve' C-m 'n' C-m

# server dev window
tmux split-window -h -t jade
tmux send-keys 'cd server && source venv/bin/activate && yarn dev' C-m

# git/misc window
tmux new-window -t jade

tmux select-window -t jade:0
tmux select-pane -t 0
tmux a -t jade
