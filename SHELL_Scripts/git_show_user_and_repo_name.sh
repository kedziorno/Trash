__red_color="\033[01;31m"
__green_color="\033[01;32m"
__yellow_color="\033[01;33m"
__blue_color="\033[01;34m"
__purple_color="\033[01;35m"
__lightgray_color="\033[00;37m"
__last_color="\033[01;00m"

function git_branch() {
    gb=$(__git_ps1 '%s')
    echo ${gb}
}

function git_userlocalglobal() {
    local exit=$?
    local ret=0
    local repo_info
    
    repo_info="$(git rev-parse --git-dir --is-inside-git-dir \
        --is-bare-repository --is-inside-work-tree \
        --short HEAD 2>/dev/null)"
    rev_parse_exit_code="$?"
    
    if [ -z "$repo_info" ]; then
        return $exit
    fi
    
    local GIT_UN_LOCAL=`git config --local user.name`;
    local GIT_UN_GLOBAL=`git config --global user.name`;
    local GIT_UE_LOCAL=`git config --local user.email`;
    local GIT_UE_GLOBAL=`git config --global user.email`;
    
    gb=$(git_branch)

    if [[ $GIT_UN_LOCAL != "" ]];
    then
        GIT_UN_LOCAL=${GIT_UN_LOCAL:-"![local username empty]!"}
        GIT_UE_LOCAL=${GIT_UE_LOCAL:-"![local useremail empty]!"}
        __git_username="[ ${gb} # (${GIT_UN_LOCAL} / ${GIT_UE_LOCAL}) ]"
        ret=100
    else
        GIT_UN_GLOBAL=${GIT_UN_GLOBAL:="![global username empty]!"}
        GIT_UE_GLOBAL=${GIT_UE_GLOBAL:="![global useremail empty]!"}
        __git_username="[ ${gb} @ (${GIT_UN_GLOBAL} / ${GIT_UE_GLOBAL}) ]"
        ret=200
    fi;
    
    if [[ ret -eq 100 ]]; then
        printf -- "${__red_color}%s"  "${__git_username}"
    fi
    if [[ ret -eq 200 ]]; then
        printf -- "${__yellow_color}%s"  "${__git_username}"
    fi
}

function set_PS1 {
    local __user_and_host="${__green_color}\u@\h \t (\#)"
    local __cur_location="${__blue_color}\w"
    local __prompt_tail="${__lightgray_color}$"
        
    export PS1="${__user_and_host} ${__cur_location} \$(git_userlocalglobal)\n${__prompt_tail} ${__last_color}"
}
set_PS1
