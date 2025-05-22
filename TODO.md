# TODO: Custom VCS Project (Upp)

## Core Features

- [x] `init` — Initialize repository  
- [ ] `status` — Show tracked/untracked/modified files  
- [ ] `add <file>` — Stage files  
- [ ] `commit -m "msg"` — Create commits  
- [ ] `log` — Show commit history  
- [ ] `branch <name>` — Create a branch  
- [ ] `checkout <branch>` — Change branches  

## Advanced Features (Planned)

- [ ] Advanced branching:
  - [ ] `branch`
  - [ ] `checkout`
  - [ ] `merge <branch>`
  - [ ] `mergetool`
- [ ] Remote sync:
  - [ ] `remote add <name> <url>` — Add a remote repository
  - [ ] `push <remote>` — Send commits to remote
  - [ ] `pull <remote>` — Fetch commits from remote
  - [ ] `clone <url>` — Clone a remote repository
- [ ] Conflict resolution:
  - [ ] Detect conflicts
  - [ ] Provide a basic conflict resolution tool
- [ ] Undoing changes:
  - [ ] `restore <file>` — Restore file to last commit
  - [ ] `reset <commit>` — Reset repository to a specific commit
- [ ] Diffing:
  - [ ] `diff` — Show differences in working directory
  - [ ] `diff --cached` — Show staged differences  

## Remote / Network Ideas

- [ ] Support for `--bare` repositories  
- [ ] Implement sync over SSH (manual or via `scp`/`rsync` initially)
- [ ] Support for multiple remotes  
- [ ] Create a minimal TCP/HTTP server for handling `push`/`pull` operations  
- [ ] Implement bundle/export system:
  - [ ] `bundle create <file>` — Export repository as a bundle
  - [ ] `bundle import <file>` — Restore repository from a bundle  

## Experimental Ideas

- [ ] Terminal UI (like `tig` or `lazygit`)  
- [ ] Hooks for custom actions (pre-commit, post-push, etc)  
- [ ] Command history tracking  

## Bugs

- Add multiple files at once
- Add all files
- Commit basic
- Commit without add
- Add non existent file
- Empty commit message
- Object Creation
