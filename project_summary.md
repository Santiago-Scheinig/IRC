# Resumen del Proyecto: IRC
Generado el: 2026-11-18 15:53:05

## Estructura de Archivos Incluidos
```
  - ./.git/description
  - ./.git/info/exclude
  - ./.git/hooks/pre-push.sample
  - ./.git/hooks/prepare-commit-msg.sample
  - ./.git/hooks/push-to-checkout.sample
  - ./.git/hooks/pre-applypatch.sample
  - ./.git/hooks/pre-receive.sample
  - ./.git/hooks/fsmonitor-watchman.sample
  - ./.git/hooks/post-update.sample
  - ./.git/hooks/pre-commit.sample
  - ./.git/hooks/commit-msg.sample
  - ./.git/hooks/update.sample
  - ./.git/hooks/pre-rebase.sample
  - ./.git/hooks/applypatch-msg.sample
  - ./.git/hooks/pre-merge-commit.sample
  - ./.git/refs/heads/main
  - ./.git/refs/heads/serg-dev
  - ./.git/refs/heads/edu-dev
  - ./.git/refs/heads/pre-dev
  - ./.git/refs/heads/santy-dev
  - ./.git/refs/heads/dev
  - ./.git/refs/remotes/origin/HEAD
  - ./.git/refs/remotes/origin/santy-dev
  - ./.git/refs/remotes/origin/serg-dev
  - ./.git/refs/remotes/origin/edu-dev
  - ./.git/refs/remotes/origin/pre-dev
  - ./.git/refs/remotes/origin/dev
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/0
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/1
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/2
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/3
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/4
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/5
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/6
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/7
  - ./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/8
  - ./.git/HEAD
  - ./.git/config
  - ./.git/packed-refs
  - ./.git/logs/refs/remotes/origin/HEAD
  - ./.git/logs/refs/remotes/origin/santy-dev
  - ./.git/logs/refs/remotes/origin/serg-dev
  - ./.git/logs/refs/remotes/origin/edu-dev
  - ./.git/logs/refs/remotes/origin/pre-dev
  - ./.git/logs/refs/remotes/origin/dev
  - ./.git/logs/refs/heads/main
  - ./.git/logs/refs/heads/serg-dev
  - ./.git/logs/refs/heads/edu-dev
  - ./.git/logs/refs/heads/pre-dev
  - ./.git/logs/refs/heads/santy-dev
  - ./.git/logs/refs/heads/dev
  - ./.git/logs/HEAD
  - ./.git/FETCH_HEAD
  - ./.git/COMMIT_EDITMSG
  - ./.git/ORIG_HEAD
  - ./.github/workflows/blank.yml
  - ./include/Channel.hpp
  - ./include/Config.hpp
  - ./include/IRCException.hpp
  - ./include/Socket.hpp
  - ./include/CommandRegistry.hpp
  - ./include/ICommand.hpp
  - ./include/Message.hpp
  - ./include/Parser.hpp
  - ./include/Replies.hpp
  - ./include/Server.hpp
  - ./src/Channel.cpp
  - ./src/Config.cpp
  - ./src/IRCException.cpp
  - ./src/Parser.cpp
  - ./src/Server.cpp
  - ./src/Socket.cpp
  - ./src/CommandRegistry.cpp
  - ./src/ICommand.cpp
  - ./src/Message.cpp
  - ./src/Replies.cpp
  - ./main.cpp
  - ./Makefile
```

## Contenido de los Archivos

### Archivo: `./.git/description`
```git/description
Unnamed repository; edit this file 'description' to name the repository.

```

### Archivo: `./.git/info/exclude`
```git/info/exclude
# git ls-files --others --exclude-from=.git/info/exclude
# Lines that start with '#' are comments.
# For a project mostly in C, the following would be a good set of
# exclude patterns (uncomment them if you want to use them):
# *.[oa]
# *~

```

### Archivo: `./.git/hooks/pre-push.sample`
```sample
#!/bin/sh

# An example hook script to verify what is about to be pushed.  Called by "git
# push" after it has checked the remote status, but before anything has been
# pushed.  If this script exits with a non-zero status nothing will be pushed.
#
# This hook is called with the following parameters:
#
# $1 -- Name of the remote to which the push is being done
# $2 -- URL to which the push is being done
#
# If pushing without using a named remote those arguments will be equal.
#
# Information about the commits which are being pushed is supplied as lines to
# the standard input in the form:
#
#   <local ref> <local oid> <remote ref> <remote oid>
#
# This sample shows how to prevent push of commits where the log message starts
# with "WIP" (work in progress).

remote="$1"
url="$2"

zero=$(git hash-object --stdin </dev/null | tr '[0-9a-f]' '0')

while read local_ref local_oid remote_ref remote_oid
do
	if test "$local_oid" = "$zero"
	then
		# Handle delete
		:
	else
		if test "$remote_oid" = "$zero"
		then
			# New branch, examine all commits
			range="$local_oid"
		else
			# Update to existing branch, examine new commits
			range="$remote_oid..$local_oid"
		fi

		# Check for WIP commit
		commit=$(git rev-list -n 1 --grep '^WIP' "$range")
		if test -n "$commit"
		then
			echo >&2 "Found WIP commit in $local_ref, not pushing"
			exit 1
		fi
	fi
done

exit 0

```

### Archivo: `./.git/hooks/prepare-commit-msg.sample`
```sample
#!/bin/sh
#
# An example hook script to prepare the commit log message.
# Called by "git commit" with the name of the file that has the
# commit message, followed by the description of the commit
# message's source.  The hook's purpose is to edit the commit
# message file.  If the hook fails with a non-zero status,
# the commit is aborted.
#
# To enable this hook, rename this file to "prepare-commit-msg".

# This hook includes three examples. The first one removes the
# "# Please enter the commit message..." help message.
#
# The second includes the output of "git diff --name-status -r"
# into the message, just before the "git status" output.  It is
# commented because it doesn't cope with --amend or with squashed
# commits.
#
# The third example adds a Signed-off-by line to the message, that can
# still be edited.  This is rarely a good idea.

COMMIT_MSG_FILE=$1
COMMIT_SOURCE=$2
SHA1=$3

/usr/bin/perl -i.bak -ne 'print unless(m/^. Please enter the commit message/..m/^#$/)' "$COMMIT_MSG_FILE"

# case "$COMMIT_SOURCE,$SHA1" in
#  ,|template,)
#    /usr/bin/perl -i.bak -pe '
#       print "\n" . `git diff --cached --name-status -r`
# 	 if /^#/ && $first++ == 0' "$COMMIT_MSG_FILE" ;;
#  *) ;;
# esac

# SOB=$(git var GIT_COMMITTER_IDENT | sed -n 's/^\(.*>\).*$/Signed-off-by: \1/p')
# git interpret-trailers --in-place --trailer "$SOB" "$COMMIT_MSG_FILE"
# if test -z "$COMMIT_SOURCE"
# then
#   /usr/bin/perl -i.bak -pe 'print "\n" if !$first_line++' "$COMMIT_MSG_FILE"
# fi

```

### Archivo: `./.git/hooks/push-to-checkout.sample`
```sample
#!/bin/sh

# An example hook script to update a checked-out tree on a git push.
#
# This hook is invoked by git-receive-pack(1) when it reacts to git
# push and updates reference(s) in its repository, and when the push
# tries to update the branch that is currently checked out and the
# receive.denyCurrentBranch configuration variable is set to
# updateInstead.
#
# By default, such a push is refused if the working tree and the index
# of the remote repository has any difference from the currently
# checked out commit; when both the working tree and the index match
# the current commit, they are updated to match the newly pushed tip
# of the branch. This hook is to be used to override the default
# behaviour; however the code below reimplements the default behaviour
# as a starting point for convenient modification.
#
# The hook receives the commit with which the tip of the current
# branch is going to be updated:
commit=$1

# It can exit with a non-zero status to refuse the push (when it does
# so, it must not modify the index or the working tree).
die () {
	echo >&2 "$*"
	exit 1
}

# Or it can make any necessary changes to the working tree and to the
# index to bring them to the desired state when the tip of the current
# branch is updated to the new commit, and exit with a zero status.
#
# For example, the hook can simply run git read-tree -u -m HEAD "$1"
# in order to emulate git fetch that is run in the reverse direction
# with git push, as the two-tree form of git read-tree -u -m is
# essentially the same as git switch or git checkout that switches
# branches while keeping the local changes in the working tree that do
# not interfere with the difference between the branches.

# The below is a more-or-less exact translation to shell of the C code
# for the default behaviour for git's push-to-checkout hook defined in
# the push_to_deploy() function in builtin/receive-pack.c.
#
# Note that the hook will be executed from the repository directory,
# not from the working tree, so if you want to perform operations on
# the working tree, you will have to adapt your code accordingly, e.g.
# by adding "cd .." or using relative paths.

if ! git update-index -q --ignore-submodules --refresh
then
	die "Up-to-date check failed"
fi

if ! git diff-files --quiet --ignore-submodules --
then
	die "Working directory has unstaged changes"
fi

# This is a rough translation of:
#
#   head_has_history() ? "HEAD" : EMPTY_TREE_SHA1_HEX
if git cat-file -e HEAD 2>/dev/null
then
	head=HEAD
else
	head=$(git hash-object -t tree --stdin </dev/null)
fi

if ! git diff-index --quiet --cached --ignore-submodules $head --
then
	die "Working directory has staged changes"
fi

if ! git read-tree -u -m "$commit"
then
	die "Could not update working tree to new HEAD"
fi

```

### Archivo: `./.git/hooks/pre-applypatch.sample`
```sample
#!/bin/sh
#
# An example hook script to verify what is about to be committed
# by applypatch from an e-mail message.
#
# The hook should exit with non-zero status after issuing an
# appropriate message if it wants to stop the commit.
#
# To enable this hook, rename this file to "pre-applypatch".

. git-sh-setup
precommit="$(git rev-parse --git-path hooks/pre-commit)"
test -x "$precommit" && exec "$precommit" ${1+"$@"}
:

```

### Archivo: `./.git/hooks/pre-receive.sample`
```sample
#!/bin/sh
#
# An example hook script to make use of push options.
# The example simply echoes all push options that start with 'echoback='
# and rejects all pushes when the "reject" push option is used.
#
# To enable this hook, rename this file to "pre-receive".

if test -n "$GIT_PUSH_OPTION_COUNT"
then
	i=0
	while test "$i" -lt "$GIT_PUSH_OPTION_COUNT"
	do
		eval "value=\$GIT_PUSH_OPTION_$i"
		case "$value" in
		echoback=*)
			echo "echo from the pre-receive-hook: ${value#*=}" >&2
			;;
		reject)
			exit 1
		esac
		i=$((i + 1))
	done
fi

```

### Archivo: `./.git/hooks/fsmonitor-watchman.sample`
```sample
#!/usr/bin/perl

use strict;
use warnings;
use IPC::Open2;

# An example hook script to integrate Watchman
# (https://facebook.github.io/watchman/) with git to speed up detecting
# new and modified files.
#
# The hook is passed a version (currently 2) and last update token
# formatted as a string and outputs to stdout a new update token and
# all files that have been modified since the update token. Paths must
# be relative to the root of the working tree and separated by a single NUL.
#
# To enable this hook, rename this file to "query-watchman" and set
# 'git config core.fsmonitor .git/hooks/query-watchman'
#
my ($version, $last_update_token) = @ARGV;

# Uncomment for debugging
# print STDERR "$0 $version $last_update_token\n";

# Check the hook interface version
if ($version ne 2) {
	die "Unsupported query-fsmonitor hook version '$version'.\n" .
	    "Falling back to scanning...\n";
}

my $git_work_tree = get_working_dir();

my $retry = 1;

my $json_pkg;
eval {
	require JSON::XS;
	$json_pkg = "JSON::XS";
	1;
} or do {
	require JSON::PP;
	$json_pkg = "JSON::PP";
};

launch_watchman();

sub launch_watchman {
	my $o = watchman_query();
	if (is_work_tree_watched($o)) {
		output_result($o->{clock}, @{$o->{files}});
	}
}

sub output_result {
	my ($clockid, @files) = @_;

	# Uncomment for debugging watchman output
	# open (my $fh, ">", ".git/watchman-output.out");
	# binmode $fh, ":utf8";
	# print $fh "$clockid\n@files\n";
	# close $fh;

	binmode STDOUT, ":utf8";
	print $clockid;
	print "\0";
	local $, = "\0";
	print @files;
}

sub watchman_clock {
	my $response = qx/watchman clock "$git_work_tree"/;
	die "Failed to get clock id on '$git_work_tree'.\n" .
		"Falling back to scanning...\n" if $? != 0;

	return $json_pkg->new->utf8->decode($response);
}

sub watchman_query {
	my $pid = open2(\*CHLD_OUT, \*CHLD_IN, 'watchman -j --no-pretty')
	or die "open2() failed: $!\n" .
	"Falling back to scanning...\n";

	# In the query expression below we're asking for names of files that
	# changed since $last_update_token but not from the .git folder.
	#
	# To accomplish this, we're using the "since" generator to use the
	# recency index to select candidate nodes and "fields" to limit the
	# output to file names only. Then we're using the "expression" term to
	# further constrain the results.
	if (substr($last_update_token, 0, 1) eq "c") {
		$last_update_token = "\"$last_update_token\"";
	}
	my $query = <<"	END";
		["query", "$git_work_tree", {
			"since": $last_update_token,
			"fields": ["name"],
			"expression": ["not", ["dirname", ".git"]]
		}]
	END

	# Uncomment for debugging the watchman query
	# open (my $fh, ">", ".git/watchman-query.json");
	# print $fh $query;
	# close $fh;

	print CHLD_IN $query;
	close CHLD_IN;
	my $response = do {local $/; <CHLD_OUT>};

	# Uncomment for debugging the watch response
	# open ($fh, ">", ".git/watchman-response.json");
	# print $fh $response;
	# close $fh;

	die "Watchman: command returned no output.\n" .
	"Falling back to scanning...\n" if $response eq "";
	die "Watchman: command returned invalid output: $response\n" .
	"Falling back to scanning...\n" unless $response =~ /^\{/;

	return $json_pkg->new->utf8->decode($response);
}

sub is_work_tree_watched {
	my ($output) = @_;
	my $error = $output->{error};
	if ($retry > 0 and $error and $error =~ m/unable to resolve root .* directory (.*) is not watched/) {
		$retry--;
		my $response = qx/watchman watch "$git_work_tree"/;
		die "Failed to make watchman watch '$git_work_tree'.\n" .
		    "Falling back to scanning...\n" if $? != 0;
		$output = $json_pkg->new->utf8->decode($response);
		$error = $output->{error};
		die "Watchman: $error.\n" .
		"Falling back to scanning...\n" if $error;

		# Uncomment for debugging watchman output
		# open (my $fh, ">", ".git/watchman-output.out");
		# close $fh;

		# Watchman will always return all files on the first query so
		# return the fast "everything is dirty" flag to git and do the
		# Watchman query just to get it over with now so we won't pay
		# the cost in git to look up each individual file.
		my $o = watchman_clock();
		$error = $output->{error};

		die "Watchman: $error.\n" .
		"Falling back to scanning...\n" if $error;

		output_result($o->{clock}, ("/"));
		$last_update_token = $o->{clock};

		eval { launch_watchman() };
		return 0;
	}

	die "Watchman: $error.\n" .
	"Falling back to scanning...\n" if $error;

	return 1;
}

sub get_working_dir {
	my $working_dir;
	if ($^O =~ 'msys' || $^O =~ 'cygwin') {
		$working_dir = Win32::GetCwd();
		$working_dir =~ tr/\\/\//;
	} else {
		require Cwd;
		$working_dir = Cwd::cwd();
	}

	return $working_dir;
}

```

### Archivo: `./.git/hooks/post-update.sample`
```sample
#!/bin/sh
#
# An example hook script to prepare a packed repository for use over
# dumb transports.
#
# To enable this hook, rename this file to "post-update".

exec git update-server-info

```

### Archivo: `./.git/hooks/pre-commit.sample`
```sample
#!/bin/sh
#
# An example hook script to verify what is about to be committed.
# Called by "git commit" with no arguments.  The hook should
# exit with non-zero status after issuing an appropriate message if
# it wants to stop the commit.
#
# To enable this hook, rename this file to "pre-commit".

if git rev-parse --verify HEAD >/dev/null 2>&1
then
	against=HEAD
else
	# Initial commit: diff against an empty tree object
	against=$(git hash-object -t tree /dev/null)
fi

# If you want to allow non-ASCII filenames set this variable to true.
allownonascii=$(git config --type=bool hooks.allownonascii)

# Redirect output to stderr.
exec 1>&2

# Cross platform projects tend to avoid non-ASCII filenames; prevent
# them from being added to the repository. We exploit the fact that the
# printable range starts at the space character and ends with tilde.
if [ "$allownonascii" != "true" ] &&
	# Note that the use of brackets around a tr range is ok here, (it's
	# even required, for portability to Solaris 10's /usr/bin/tr), since
	# the square bracket bytes happen to fall in the designated range.
	test $(git diff --cached --name-only --diff-filter=A -z $against |
	  LC_ALL=C tr -d '[ -~]\0' | wc -c) != 0
then
	cat <<\EOF
Error: Attempt to add a non-ASCII file name.

This can cause problems if you want to work with people on other platforms.

To be portable it is advisable to rename the file.

If you know what you are doing you can disable this check using:

  git config hooks.allownonascii true
EOF
	exit 1
fi

# If there are whitespace errors, print the offending file names and fail.
exec git diff-index --check --cached $against --

```

### Archivo: `./.git/hooks/commit-msg.sample`
```sample
#!/bin/sh
#
# An example hook script to check the commit log message.
# Called by "git commit" with one argument, the name of the file
# that has the commit message.  The hook should exit with non-zero
# status after issuing an appropriate message if it wants to stop the
# commit.  The hook is allowed to edit the commit message file.
#
# To enable this hook, rename this file to "commit-msg".

# Uncomment the below to add a Signed-off-by line to the message.
# Doing this in a hook is a bad idea in general, but the prepare-commit-msg
# hook is more suited to it.
#
# SOB=$(git var GIT_AUTHOR_IDENT | sed -n 's/^\(.*>\).*$/Signed-off-by: \1/p')
# grep -qs "^$SOB" "$1" || echo "$SOB" >> "$1"

# This example catches duplicate Signed-off-by lines.

test "" = "$(grep '^Signed-off-by: ' "$1" |
	 sort | uniq -c | sed -e '/^[ 	]*1[ 	]/d')" || {
	echo >&2 Duplicate Signed-off-by lines.
	exit 1
}

```

### Archivo: `./.git/hooks/update.sample`
```sample
#!/bin/sh
#
# An example hook script to block unannotated tags from entering.
# Called by "git receive-pack" with arguments: refname sha1-old sha1-new
#
# To enable this hook, rename this file to "update".
#
# Config
# ------
# hooks.allowunannotated
#   This boolean sets whether unannotated tags will be allowed into the
#   repository.  By default they won't be.
# hooks.allowdeletetag
#   This boolean sets whether deleting tags will be allowed in the
#   repository.  By default they won't be.
# hooks.allowmodifytag
#   This boolean sets whether a tag may be modified after creation. By default
#   it won't be.
# hooks.allowdeletebranch
#   This boolean sets whether deleting branches will be allowed in the
#   repository.  By default they won't be.
# hooks.denycreatebranch
#   This boolean sets whether remotely creating branches will be denied
#   in the repository.  By default this is allowed.
#

# --- Command line
refname="$1"
oldrev="$2"
newrev="$3"

# --- Safety check
if [ -z "$GIT_DIR" ]; then
	echo "Don't run this script from the command line." >&2
	echo " (if you want, you could supply GIT_DIR then run" >&2
	echo "  $0 <ref> <oldrev> <newrev>)" >&2
	exit 1
fi

if [ -z "$refname" -o -z "$oldrev" -o -z "$newrev" ]; then
	echo "usage: $0 <ref> <oldrev> <newrev>" >&2
	exit 1
fi

# --- Config
allowunannotated=$(git config --type=bool hooks.allowunannotated)
allowdeletebranch=$(git config --type=bool hooks.allowdeletebranch)
denycreatebranch=$(git config --type=bool hooks.denycreatebranch)
allowdeletetag=$(git config --type=bool hooks.allowdeletetag)
allowmodifytag=$(git config --type=bool hooks.allowmodifytag)

# check for no description
projectdesc=$(sed -e '1q' "$GIT_DIR/description")
case "$projectdesc" in
"Unnamed repository"* | "")
	echo "*** Project description file hasn't been set" >&2
	exit 1
	;;
esac

# --- Check types
# if $newrev is 0000...0000, it's a commit to delete a ref.
zero=$(git hash-object --stdin </dev/null | tr '[0-9a-f]' '0')
if [ "$newrev" = "$zero" ]; then
	newrev_type=delete
else
	newrev_type=$(git cat-file -t $newrev)
fi

case "$refname","$newrev_type" in
	refs/tags/*,commit)
		# un-annotated tag
		short_refname=${refname##refs/tags/}
		if [ "$allowunannotated" != "true" ]; then
			echo "*** The un-annotated tag, $short_refname, is not allowed in this repository" >&2
			echo "*** Use 'git tag [ -a | -s ]' for tags you want to propagate." >&2
			exit 1
		fi
		;;
	refs/tags/*,delete)
		# delete tag
		if [ "$allowdeletetag" != "true" ]; then
			echo "*** Deleting a tag is not allowed in this repository" >&2
			exit 1
		fi
		;;
	refs/tags/*,tag)
		# annotated tag
		if [ "$allowmodifytag" != "true" ] && git rev-parse $refname > /dev/null 2>&1
		then
			echo "*** Tag '$refname' already exists." >&2
			echo "*** Modifying a tag is not allowed in this repository." >&2
			exit 1
		fi
		;;
	refs/heads/*,commit)
		# branch
		if [ "$oldrev" = "$zero" -a "$denycreatebranch" = "true" ]; then
			echo "*** Creating a branch is not allowed in this repository" >&2
			exit 1
		fi
		;;
	refs/heads/*,delete)
		# delete branch
		if [ "$allowdeletebranch" != "true" ]; then
			echo "*** Deleting a branch is not allowed in this repository" >&2
			exit 1
		fi
		;;
	refs/remotes/*,commit)
		# tracking branch
		;;
	refs/remotes/*,delete)
		# delete tracking branch
		if [ "$allowdeletebranch" != "true" ]; then
			echo "*** Deleting a tracking branch is not allowed in this repository" >&2
			exit 1
		fi
		;;
	*)
		# Anything else (is there anything else?)
		echo "*** Update hook: unknown type of update to ref $refname of type $newrev_type" >&2
		exit 1
		;;
esac

# --- Finished
exit 0

```

### Archivo: `./.git/hooks/pre-rebase.sample`
```sample
#!/bin/sh
#
# Copyright (c) 2006, 2008 Junio C Hamano
#
# The "pre-rebase" hook is run just before "git rebase" starts doing
# its job, and can prevent the command from running by exiting with
# non-zero status.
#
# The hook is called with the following parameters:
#
# $1 -- the upstream the series was forked from.
# $2 -- the branch being rebased (or empty when rebasing the current branch).
#
# This sample shows how to prevent topic branches that are already
# merged to 'next' branch from getting rebased, because allowing it
# would result in rebasing already published history.

publish=next
basebranch="$1"
if test "$#" = 2
then
	topic="refs/heads/$2"
else
	topic=`git symbolic-ref HEAD` ||
	exit 0 ;# we do not interrupt rebasing detached HEAD
fi

case "$topic" in
refs/heads/??/*)
	;;
*)
	exit 0 ;# we do not interrupt others.
	;;
esac

# Now we are dealing with a topic branch being rebased
# on top of master.  Is it OK to rebase it?

# Does the topic really exist?
git show-ref -q "$topic" || {
	echo >&2 "No such branch $topic"
	exit 1
}

# Is topic fully merged to master?
not_in_master=`git rev-list --pretty=oneline ^master "$topic"`
if test -z "$not_in_master"
then
	echo >&2 "$topic is fully merged to master; better remove it."
	exit 1 ;# we could allow it, but there is no point.
fi

# Is topic ever merged to next?  If so you should not be rebasing it.
only_next_1=`git rev-list ^master "^$topic" ${publish} | sort`
only_next_2=`git rev-list ^master           ${publish} | sort`
if test "$only_next_1" = "$only_next_2"
then
	not_in_topic=`git rev-list "^$topic" master`
	if test -z "$not_in_topic"
	then
		echo >&2 "$topic is already up to date with master"
		exit 1 ;# we could allow it, but there is no point.
	else
		exit 0
	fi
else
	not_in_next=`git rev-list --pretty=oneline ^${publish} "$topic"`
	/usr/bin/perl -e '
		my $topic = $ARGV[0];
		my $msg = "* $topic has commits already merged to public branch:\n";
		my (%not_in_next) = map {
			/^([0-9a-f]+) /;
			($1 => 1);
		} split(/\n/, $ARGV[1]);
		for my $elem (map {
				/^([0-9a-f]+) (.*)$/;
				[$1 => $2];
			} split(/\n/, $ARGV[2])) {
			if (!exists $not_in_next{$elem->[0]}) {
				if ($msg) {
					print STDERR $msg;
					undef $msg;
				}
				print STDERR " $elem->[1]\n";
			}
		}
	' "$topic" "$not_in_next" "$not_in_master"
	exit 1
fi

<<\DOC_END

This sample hook safeguards topic branches that have been
published from being rewound.

The workflow assumed here is:

 * Once a topic branch forks from "master", "master" is never
   merged into it again (either directly or indirectly).

 * Once a topic branch is fully cooked and merged into "master",
   it is deleted.  If you need to build on top of it to correct
   earlier mistakes, a new topic branch is created by forking at
   the tip of the "master".  This is not strictly necessary, but
   it makes it easier to keep your history simple.

 * Whenever you need to test or publish your changes to topic
   branches, merge them into "next" branch.

The script, being an example, hardcodes the publish branch name
to be "next", but it is trivial to make it configurable via
$GIT_DIR/config mechanism.

With this workflow, you would want to know:

(1) ... if a topic branch has ever been merged to "next".  Young
    topic branches can have stupid mistakes you would rather
    clean up before publishing, and things that have not been
    merged into other branches can be easily rebased without
    affecting other people.  But once it is published, you would
    not want to rewind it.

(2) ... if a topic branch has been fully merged to "master".
    Then you can delete it.  More importantly, you should not
    build on top of it -- other people may already want to
    change things related to the topic as patches against your
    "master", so if you need further changes, it is better to
    fork the topic (perhaps with the same name) afresh from the
    tip of "master".

Let's look at this example:

		   o---o---o---o---o---o---o---o---o---o "next"
		  /       /           /           /
		 /   a---a---b A     /           /
		/   /               /           /
	       /   /   c---c---c---c B         /
	      /   /   /             \         /
	     /   /   /   b---b C     \       /
	    /   /   /   /             \     /
    ---o---o---o---o---o---o---o---o---o---o---o "master"


A, B and C are topic branches.

 * A has one fix since it was merged up to "next".

 * B has finished.  It has been fully merged up to "master" and "next",
   and is ready to be deleted.

 * C has not merged to "next" at all.

We would want to allow C to be rebased, refuse A, and encourage
B to be deleted.

To compute (1):

	git rev-list ^master ^topic next
	git rev-list ^master        next

	if these match, topic has not merged in next at all.

To compute (2):

	git rev-list master..topic

	if this is empty, it is fully merged to "master".

DOC_END

```

### Archivo: `./.git/hooks/applypatch-msg.sample`
```sample
#!/bin/sh
#
# An example hook script to check the commit log message taken by
# applypatch from an e-mail message.
#
# The hook should exit with non-zero status after issuing an
# appropriate message if it wants to stop the commit.  The hook is
# allowed to edit the commit message file.
#
# To enable this hook, rename this file to "applypatch-msg".

. git-sh-setup
commitmsg="$(git rev-parse --git-path hooks/commit-msg)"
test -x "$commitmsg" && exec "$commitmsg" ${1+"$@"}
:

```

### Archivo: `./.git/hooks/pre-merge-commit.sample`
```sample
#!/bin/sh
#
# An example hook script to verify what is about to be committed.
# Called by "git merge" with no arguments.  The hook should
# exit with non-zero status after issuing an appropriate message to
# stderr if it wants to stop the merge commit.
#
# To enable this hook, rename this file to "pre-merge-commit".

. git-sh-setup
test -x "$GIT_DIR/hooks/pre-commit" &&
        exec "$GIT_DIR/hooks/pre-commit"
:

```

### Archivo: `./.git/refs/heads/main`
```git/refs/heads/main
813f32784d339369863d06d0fbc0cbcf33e7c6af

```

### Archivo: `./.git/refs/heads/serg-dev`
```git/refs/heads/serg-dev
50c6a713af66170ce05473e42f04e21d90b9def9

```

### Archivo: `./.git/refs/heads/edu-dev`
```git/refs/heads/edu-dev
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec

```

### Archivo: `./.git/refs/heads/pre-dev`
```git/refs/heads/pre-dev
a4345196abe9a4320deceba96d68ad2b3225bd16

```

### Archivo: `./.git/refs/heads/santy-dev`
```git/refs/heads/santy-dev
48a9fd143030d1873098b876101c6316dc531500

```

### Archivo: `./.git/refs/heads/dev`
```git/refs/heads/dev
1fc2182618796c68c151d3f9f3f965670150a183

```

### Archivo: `./.git/refs/remotes/origin/HEAD`
```git/refs/remotes/origin/HEAD
ref: refs/remotes/origin/main

```

### Archivo: `./.git/refs/remotes/origin/santy-dev`
```git/refs/remotes/origin/santy-dev
48a9fd143030d1873098b876101c6316dc531500

```

### Archivo: `./.git/refs/remotes/origin/serg-dev`
```git/refs/remotes/origin/serg-dev
50c6a713af66170ce05473e42f04e21d90b9def9

```

### Archivo: `./.git/refs/remotes/origin/edu-dev`
```git/refs/remotes/origin/edu-dev
c04993bb49c1e5207db25e23ae5831e3ae41ad80

```

### Archivo: `./.git/refs/remotes/origin/pre-dev`
```git/refs/remotes/origin/pre-dev
8509036cabe94b0447c2ba171aa347505c7f9121

```

### Archivo: `./.git/refs/remotes/origin/dev`
```git/refs/remotes/origin/dev
1fc2182618796c68c151d3f9f3f965670150a183

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/0`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/0
755e7151c3fd0538562de77fe0234c7067a0cb61

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/1`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/1
c0715bb6147197601dc3e28d9fcb6c334a9acd0c

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/2`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/2
2c80318c9979c3f7e9b9ba07f26f053a487d71df

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/3`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/3
1395391e781e39508417364d7290bd33a6e5d706

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/4`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/4
6eb63f1ca187d31b7cd465ddf7429365d4eb9c30

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/5`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/5
fa7a09b8c1c7a339e24e6510bc6108fbfb47a604

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/6`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/6
0989a2243a095a98c8f2c51114e4e07ca6cac530

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/7`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/7
1a501e9e7392140ba1de3e54bfae16107e380a3b

```

### Archivo: `./.git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/8`
```git/refs/agents/9b17d74b-f163-4533-a5b1-401aca0bc4ac/checkpoints/turn/8
1d2802bf9c5cfb69e10849cbef9c48f444c5191c

```

### Archivo: `./.git/HEAD`
```git/HEAD
ref: refs/heads/serg-dev

```

### Archivo: `./.git/config`
```git/config
[core]
	repositoryformatversion = 0
	filemode = true
	bare = false
	logallrefupdates = true
[remote "origin"]
	url = git@github.com:Santiago-Scheinig/IRC.git
	fetch = +refs/heads/*:refs/remotes/origin/*
[branch "main"]
	remote = origin
	merge = refs/heads/main
	vscode-merge-base = origin/main
[branch "serg-dev"]
	vscode-merge-base = origin/main
	remote = origin
	merge = refs/heads/serg-dev
[branch "edu-dev"]
	remote = origin
	merge = refs/heads/edu-dev
	vscode-merge-base = origin/edu-dev
[branch "pre-dev"]
	remote = origin
	merge = refs/heads/pre-dev
	vscode-merge-base = origin/pre-dev
[branch "santy-dev"]
	remote = origin
	merge = refs/heads/santy-dev
	vscode-merge-base = origin/santy-dev
[branch "dev"]
	remote = origin
	merge = refs/heads/dev
	vscode-merge-base = origin/dev

```

### Archivo: `./.git/packed-refs`
```git/packed-refs
# pack-refs with: peeled fully-peeled sorted 
813f32784d339369863d06d0fbc0cbcf33e7c6af refs/remotes/origin/dev
24c2e8573ea95849f68f86c1f208dbf411a6bb98 refs/remotes/origin/edu-dev
813f32784d339369863d06d0fbc0cbcf33e7c6af refs/remotes/origin/main
947822a856f231bca8df3cab246137e37bad02aa refs/remotes/origin/pre-dev

```

### Archivo: `./.git/logs/refs/remotes/origin/HEAD`
```git/logs/refs/remotes/origin/HEAD
0000000000000000000000000000000000000000 813f32784d339369863d06d0fbc0cbcf33e7c6af ssoto-su <ssoto-su@student.42malaga.com> 1790602505 +0200	clone: from github.com:Santiago-Scheinig/IRC.git

```

### Archivo: `./.git/logs/refs/remotes/origin/santy-dev`
```git/logs/refs/remotes/origin/santy-dev
0000000000000000000000000000000000000000 50050ce52f756ac2100491891ea9e852e56fb80d ssoto-su <ssoto-su@student.42malaga.com> 1790686910 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: storing head
50050ce52f756ac2100491891ea9e852e56fb80d 2b3094d3288e2718ecdb758a227a50e9742f9dfd ssoto-su <ssoto-su@student.42malaga.com> 1792416849 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
2b3094d3288e2718ecdb758a227a50e9742f9dfd 092e75c3c928868b12c47dc1c455d786fe3bc78d ssoto-su <ssoto-su@student.42malaga.com> 1794482953 +0100	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
092e75c3c928868b12c47dc1c455d786fe3bc78d 48a9fd143030d1873098b876101c6316dc531500 ssoto-su <ssoto-su@student.42malaga.com> 1795011415 +0100	pull --tags origin santy-dev: fast-forward

```

### Archivo: `./.git/logs/refs/remotes/origin/serg-dev`
```git/logs/refs/remotes/origin/serg-dev
0000000000000000000000000000000000000000 24a4dc1d42c0086be6da71b6504ca3b1c643d5f5 ssoto-su <ssoto-su@student.42malaga.com> 1790690360 +0200	update by push
24a4dc1d42c0086be6da71b6504ca3b1c643d5f5 e15e1c204d5ef17cfe89e3ee3f5ad748b46a06a4 ssoto-su <ssoto-su@student.42malaga.com> 1790780550 +0200	update by push
e15e1c204d5ef17cfe89e3ee3f5ad748b46a06a4 7970633ae0867d316ac7f58562f8375676b0d4f1 ssoto-su <ssoto-su@student.42malaga.com> 1791809598 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
7970633ae0867d316ac7f58562f8375676b0d4f1 77f978d0636bc77598110d03fa89793102e7ffe5 ssoto-su <ssoto-su@student.42malaga.com> 1792682437 +0200	update by push
77f978d0636bc77598110d03fa89793102e7ffe5 23fefde9c3ebb95663cd8968d3462a10d1e11ecd ssoto-su <ssoto-su@student.42malaga.com> 1793018411 +0100	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
23fefde9c3ebb95663cd8968d3462a10d1e11ecd 06a86e64bece8da0cb752efe018ee354a45dad08 ssoto-su <ssoto-su@student.42malaga.com> 1793117606 +0100	update by push
06a86e64bece8da0cb752efe018ee354a45dad08 2dd3ad36a3e35cca40f6d4c85ba8548e4b7cd21c ssoto-su <ssoto-su@student.42malaga.com> 1793294920 +0100	update by push
2dd3ad36a3e35cca40f6d4c85ba8548e4b7cd21c 6e710cd560f5cbe5449dcbd6948f89b931009304 ssoto-su <ssoto-su@student.42malaga.com> 1793364864 +0100	update by push
6e710cd560f5cbe5449dcbd6948f89b931009304 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793797844 +0100	update by push
64589e03a91e1d095a3dcdb38de67d40a870f696 9c24aeef03b4731178b0fa0ee095f01bbd3a70b2 ssoto-su <ssoto-su@student.42malaga.com> 1794482945 +0100	pull --tags origin serg-dev: fast-forward
9c24aeef03b4731178b0fa0ee095f01bbd3a70b2 7886cfd84f9dabd17d3e628be588523325276883 ssoto-su <ssoto-su@student.42malaga.com> 1794495125 +0100	update by push
7886cfd84f9dabd17d3e628be588523325276883 50c6a713af66170ce05473e42f04e21d90b9def9 ssoto-su <ssoto-su@student.42malaga.com> 1795011402 +0100	pull --tags origin serg-dev: fast-forward

```

### Archivo: `./.git/logs/refs/remotes/origin/edu-dev`
```git/logs/refs/remotes/origin/edu-dev
24c2e8573ea95849f68f86c1f208dbf411a6bb98 fe15267743f957df557acb8e1da4cccb729ccae1 ssoto-su <ssoto-su@student.42malaga.com> 1791809598 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
fe15267743f957df557acb8e1da4cccb729ccae1 38404870cb4a2de8ad0288bd7c1e137376be8446 ssoto-su <ssoto-su@student.42malaga.com> 1792416849 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
38404870cb4a2de8ad0288bd7c1e137376be8446 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793018411 +0100	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec c04993bb49c1e5207db25e23ae5831e3ae41ad80 ssoto-su <ssoto-su@student.42malaga.com> 1794482953 +0100	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward

```

### Archivo: `./.git/logs/refs/remotes/origin/pre-dev`
```git/logs/refs/remotes/origin/pre-dev
947822a856f231bca8df3cab246137e37bad02aa f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 ssoto-su <ssoto-su@student.42malaga.com> 1791809598 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 a4345196abe9a4320deceba96d68ad2b3225bd16 ssoto-su <ssoto-su@student.42malaga.com> 1792425724 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward
a4345196abe9a4320deceba96d68ad2b3225bd16 8509036cabe94b0447c2ba171aa347505c7f9121 ssoto-su <ssoto-su@student.42malaga.com> 1794482953 +0100	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward

```

### Archivo: `./.git/logs/refs/remotes/origin/dev`
```git/logs/refs/remotes/origin/dev
813f32784d339369863d06d0fbc0cbcf33e7c6af 1fc2182618796c68c151d3f9f3f965670150a183 ssoto-su <ssoto-su@student.42malaga.com> 1792425724 +0200	fetch --append --no-auto-gc --no-write-commit-graph origin: fast-forward

```

### Archivo: `./.git/logs/refs/heads/main`
```git/logs/refs/heads/main
0000000000000000000000000000000000000000 813f32784d339369863d06d0fbc0cbcf33e7c6af ssoto-su <ssoto-su@student.42malaga.com> 1790602505 +0200	clone: from github.com:Santiago-Scheinig/IRC.git

```

### Archivo: `./.git/logs/refs/heads/serg-dev`
```git/logs/refs/heads/serg-dev
0000000000000000000000000000000000000000 813f32784d339369863d06d0fbc0cbcf33e7c6af ssoto-su <ssoto-su@student.42malaga.com> 1790688002 +0200	branch: Created from origin/main
813f32784d339369863d06d0fbc0cbcf33e7c6af 24a4dc1d42c0086be6da71b6504ca3b1c643d5f5 ssoto-su <ssoto-su@student.42malaga.com> 1790690351 +0200	commit: feat(Client) begins the class structured
24a4dc1d42c0086be6da71b6504ca3b1c643d5f5 e15e1c204d5ef17cfe89e3ee3f5ad748b46a06a4 ssoto-su <ssoto-su@student.42malaga.com> 1790780547 +0200	commit: feat(#5) Client.cpp added
e15e1c204d5ef17cfe89e3ee3f5ad748b46a06a4 7970633ae0867d316ac7f58562f8375676b0d4f1 ssoto-su <ssoto-su@student.42malaga.com> 1791809767 +0200	pull --tags origin serg-dev: Fast-forward
7970633ae0867d316ac7f58562f8375676b0d4f1 732008e73a3cad3db5c05b3e7f224b248b4067d8 ssoto-su <ssoto-su@student.42malaga.com> 1792426288 +0200	merge pre-dev: Merge made by the 'ort' strategy.
732008e73a3cad3db5c05b3e7f224b248b4067d8 8c96db1f970e6510f19bd4d84942e4b571810821 ssoto-su <ssoto-su@student.42malaga.com> 1792505028 +0200	commit: feat(Client): few changes on Client to work on new logic with Parser
8c96db1f970e6510f19bd4d84942e4b571810821 f9c0f15f5a69e582f384d1d68b30931b26da5efe ssoto-su <ssoto-su@student.42malaga.com> 1792508899 +0200	commit: feat(Client): Going back to the first logic, Parser is the one to receive the data from the buffer, Client only calls Parser::processInput()
f9c0f15f5a69e582f384d1d68b30931b26da5efe 9481eac533b850f7d9198d6c2c89941688030059 ssoto-su <ssoto-su@student.42malaga.com> 1792515051 +0200	commit: feat(Server): work in progress
9481eac533b850f7d9198d6c2c89941688030059 64069cbefb07657f813970491758fa162e900a25 ssoto-su <ssoto-su@student.42malaga.com> 1792681173 +0200	commit: feat(Server): new funtions initServer and run added
64069cbefb07657f813970491758fa162e900a25 77f978d0636bc77598110d03fa89793102e7ffe5 ssoto-su <ssoto-su@student.42malaga.com> 1792681819 +0200	commit: feat(Server): New function deleteFd added
77f978d0636bc77598110d03fa89793102e7ffe5 23fefde9c3ebb95663cd8968d3462a10d1e11ecd ssoto-su <ssoto-su@student.42malaga.com> 1793018424 +0100	pull --tags origin serg-dev: Fast-forward
23fefde9c3ebb95663cd8968d3462a10d1e11ecd eacdb0fe3d56ca59f0788bd6663bf889b8187ff8 ssoto-su <ssoto-su@student.42malaga.com> 1793112490 +0100	commit: feat(Client): 2 new functions added to handle the poll status POLLOUT or POLLIN
eacdb0fe3d56ca59f0788bd6663bf889b8187ff8 06a86e64bece8da0cb752efe018ee354a45dad08 ssoto-su <ssoto-su@student.42malaga.com> 1793117587 +0100	commit: feat(Server): New funtions added, acceptNewClient, sendData, enableWrite
06a86e64bece8da0cb752efe018ee354a45dad08 469dbf4c9434d20e8bb8fb036926659634020479 ssoto-su <ssoto-su@student.42malaga.com> 1793287799 +0100	commit: feat(Client): setters and getters for Client, also 2 new functions for client's autheentication logic using in Server.
469dbf4c9434d20e8bb8fb036926659634020479 2dd3ad36a3e35cca40f6d4c85ba8548e4b7cd21c ssoto-su <ssoto-su@student.42malaga.com> 1793294898 +0100	commit: feat(Server): new function passwordMatches should be called for PASS command.
2dd3ad36a3e35cca40f6d4c85ba8548e4b7cd21c 6e710cd560f5cbe5449dcbd6948f89b931009304 ssoto-su <ssoto-su@student.42malaga.com> 1793364851 +0100	commit: feat(Server): just two new conditions just in case the bind or listen fails must return _serverFd to its default value
6e710cd560f5cbe5449dcbd6948f89b931009304 ad8552ef409a3db75335966fb824de490a5ede0e ssoto-su <ssoto-su@student.42malaga.com> 1793377655 +0100	commit: fix(Server): sendData now returns bool, added another condition when a message is incomplete does the other side of the pipe just close does not wait for the rest of the message.
ad8552ef409a3db75335966fb824de490a5ede0e 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793631747 +0100	commit: Fix(Server): Fix the logic and send and receiveData, no check in on errno var to decide what to do next.
64589e03a91e1d095a3dcdb38de67d40a870f696 9c24aeef03b4731178b0fa0ee095f01bbd3a70b2 ssoto-su <ssoto-su@student.42malaga.com> 1794482945 +0100	pull --tags origin serg-dev: Fast-forward
9c24aeef03b4731178b0fa0ee095f01bbd3a70b2 7886cfd84f9dabd17d3e628be588523325276883 ssoto-su <ssoto-su@student.42malaga.com> 1794495109 +0100	commit: add(Server): New fucn added removeClientFromChannels. when a client quit from server. this function looks for the client in all channel and delete it.
7886cfd84f9dabd17d3e628be588523325276883 50c6a713af66170ce05473e42f04e21d90b9def9 ssoto-su <ssoto-su@student.42malaga.com> 1795011402 +0100	pull --tags origin serg-dev: Fast-forward

```

### Archivo: `./.git/logs/refs/heads/edu-dev`
```git/logs/refs/heads/edu-dev
0000000000000000000000000000000000000000 fe15267743f957df557acb8e1da4cccb729ccae1 ssoto-su <ssoto-su@student.42malaga.com> 1791815438 +0200	branch: Created from origin/edu-dev
fe15267743f957df557acb8e1da4cccb729ccae1 38404870cb4a2de8ad0288bd7c1e137376be8446 ssoto-su <ssoto-su@student.42malaga.com> 1792417554 +0200	pull --tags origin edu-dev: Fast-forward
38404870cb4a2de8ad0288bd7c1e137376be8446 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793797962 +0100	pull --tags origin edu-dev: Fast-forward

```

### Archivo: `./.git/logs/refs/heads/pre-dev`
```git/logs/refs/heads/pre-dev
0000000000000000000000000000000000000000 f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 ssoto-su <ssoto-su@student.42malaga.com> 1792417455 +0200	branch: Created from origin/pre-dev
f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 a4345196abe9a4320deceba96d68ad2b3225bd16 ssoto-su <ssoto-su@student.42malaga.com> 1793801639 +0100	pull --tags origin pre-dev: Fast-forward

```

### Archivo: `./.git/logs/refs/heads/santy-dev`
```git/logs/refs/heads/santy-dev
0000000000000000000000000000000000000000 2b3094d3288e2718ecdb758a227a50e9742f9dfd ssoto-su <ssoto-su@student.42malaga.com> 1793798451 +0100	branch: Created from origin/santy-dev
2b3094d3288e2718ecdb758a227a50e9742f9dfd 48a9fd143030d1873098b876101c6316dc531500 ssoto-su <ssoto-su@student.42malaga.com> 1795011416 +0100	pull --tags origin santy-dev: Fast-forward

```

### Archivo: `./.git/logs/refs/heads/dev`
```git/logs/refs/heads/dev
0000000000000000000000000000000000000000 1fc2182618796c68c151d3f9f3f965670150a183 ssoto-su <ssoto-su@student.42malaga.com> 1793885908 +0100	branch: Created from origin/dev

```

### Archivo: `./.git/logs/HEAD`
```git/logs/HEAD
0000000000000000000000000000000000000000 813f32784d339369863d06d0fbc0cbcf33e7c6af ssoto-su <ssoto-su@student.42malaga.com> 1790602505 +0200	clone: from github.com:Santiago-Scheinig/IRC.git
813f32784d339369863d06d0fbc0cbcf33e7c6af 813f32784d339369863d06d0fbc0cbcf33e7c6af ssoto-su <ssoto-su@student.42malaga.com> 1790688002 +0200	checkout: moving from main to serg-dev
813f32784d339369863d06d0fbc0cbcf33e7c6af 24a4dc1d42c0086be6da71b6504ca3b1c643d5f5 ssoto-su <ssoto-su@student.42malaga.com> 1790690351 +0200	commit: feat(Client) begins the class structured
24a4dc1d42c0086be6da71b6504ca3b1c643d5f5 e15e1c204d5ef17cfe89e3ee3f5ad748b46a06a4 ssoto-su <ssoto-su@student.42malaga.com> 1790780547 +0200	commit: feat(#5) Client.cpp added
e15e1c204d5ef17cfe89e3ee3f5ad748b46a06a4 7970633ae0867d316ac7f58562f8375676b0d4f1 ssoto-su <ssoto-su@student.42malaga.com> 1791809767 +0200	pull --tags origin serg-dev: Fast-forward
7970633ae0867d316ac7f58562f8375676b0d4f1 fe15267743f957df557acb8e1da4cccb729ccae1 ssoto-su <ssoto-su@student.42malaga.com> 1791815438 +0200	checkout: moving from serg-dev to edu-dev
fe15267743f957df557acb8e1da4cccb729ccae1 7970633ae0867d316ac7f58562f8375676b0d4f1 ssoto-su <ssoto-su@student.42malaga.com> 1791816261 +0200	checkout: moving from edu-dev to serg-dev
7970633ae0867d316ac7f58562f8375676b0d4f1 fe15267743f957df557acb8e1da4cccb729ccae1 ssoto-su <ssoto-su@student.42malaga.com> 1792417024 +0200	checkout: moving from serg-dev to edu-dev
fe15267743f957df557acb8e1da4cccb729ccae1 7970633ae0867d316ac7f58562f8375676b0d4f1 ssoto-su <ssoto-su@student.42malaga.com> 1792417067 +0200	checkout: moving from edu-dev to serg-dev
7970633ae0867d316ac7f58562f8375676b0d4f1 f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 ssoto-su <ssoto-su@student.42malaga.com> 1792417455 +0200	checkout: moving from serg-dev to pre-dev
f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 fe15267743f957df557acb8e1da4cccb729ccae1 ssoto-su <ssoto-su@student.42malaga.com> 1792417514 +0200	checkout: moving from pre-dev to edu-dev
fe15267743f957df557acb8e1da4cccb729ccae1 38404870cb4a2de8ad0288bd7c1e137376be8446 ssoto-su <ssoto-su@student.42malaga.com> 1792417554 +0200	pull --tags origin edu-dev: Fast-forward
38404870cb4a2de8ad0288bd7c1e137376be8446 7970633ae0867d316ac7f58562f8375676b0d4f1 ssoto-su <ssoto-su@student.42malaga.com> 1792419923 +0200	checkout: moving from edu-dev to serg-dev
7970633ae0867d316ac7f58562f8375676b0d4f1 732008e73a3cad3db5c05b3e7f224b248b4067d8 ssoto-su <ssoto-su@student.42malaga.com> 1792426288 +0200	merge pre-dev: Merge made by the 'ort' strategy.
732008e73a3cad3db5c05b3e7f224b248b4067d8 8c96db1f970e6510f19bd4d84942e4b571810821 ssoto-su <ssoto-su@student.42malaga.com> 1792505028 +0200	commit: feat(Client): few changes on Client to work on new logic with Parser
8c96db1f970e6510f19bd4d84942e4b571810821 f9c0f15f5a69e582f384d1d68b30931b26da5efe ssoto-su <ssoto-su@student.42malaga.com> 1792508899 +0200	commit: feat(Client): Going back to the first logic, Parser is the one to receive the data from the buffer, Client only calls Parser::processInput()
f9c0f15f5a69e582f384d1d68b30931b26da5efe 9481eac533b850f7d9198d6c2c89941688030059 ssoto-su <ssoto-su@student.42malaga.com> 1792515051 +0200	commit: feat(Server): work in progress
9481eac533b850f7d9198d6c2c89941688030059 64069cbefb07657f813970491758fa162e900a25 ssoto-su <ssoto-su@student.42malaga.com> 1792681173 +0200	commit: feat(Server): new funtions initServer and run added
64069cbefb07657f813970491758fa162e900a25 77f978d0636bc77598110d03fa89793102e7ffe5 ssoto-su <ssoto-su@student.42malaga.com> 1792681819 +0200	commit: feat(Server): New function deleteFd added
77f978d0636bc77598110d03fa89793102e7ffe5 23fefde9c3ebb95663cd8968d3462a10d1e11ecd ssoto-su <ssoto-su@student.42malaga.com> 1793018424 +0100	pull --tags origin serg-dev: Fast-forward
23fefde9c3ebb95663cd8968d3462a10d1e11ecd eacdb0fe3d56ca59f0788bd6663bf889b8187ff8 ssoto-su <ssoto-su@student.42malaga.com> 1793112490 +0100	commit: feat(Client): 2 new functions added to handle the poll status POLLOUT or POLLIN
eacdb0fe3d56ca59f0788bd6663bf889b8187ff8 06a86e64bece8da0cb752efe018ee354a45dad08 ssoto-su <ssoto-su@student.42malaga.com> 1793117587 +0100	commit: feat(Server): New funtions added, acceptNewClient, sendData, enableWrite
06a86e64bece8da0cb752efe018ee354a45dad08 469dbf4c9434d20e8bb8fb036926659634020479 ssoto-su <ssoto-su@student.42malaga.com> 1793287799 +0100	commit: feat(Client): setters and getters for Client, also 2 new functions for client's autheentication logic using in Server.
469dbf4c9434d20e8bb8fb036926659634020479 2dd3ad36a3e35cca40f6d4c85ba8548e4b7cd21c ssoto-su <ssoto-su@student.42malaga.com> 1793294898 +0100	commit: feat(Server): new function passwordMatches should be called for PASS command.
2dd3ad36a3e35cca40f6d4c85ba8548e4b7cd21c 6e710cd560f5cbe5449dcbd6948f89b931009304 ssoto-su <ssoto-su@student.42malaga.com> 1793364851 +0100	commit: feat(Server): just two new conditions just in case the bind or listen fails must return _serverFd to its default value
6e710cd560f5cbe5449dcbd6948f89b931009304 ad8552ef409a3db75335966fb824de490a5ede0e ssoto-su <ssoto-su@student.42malaga.com> 1793377655 +0100	commit: fix(Server): sendData now returns bool, added another condition when a message is incomplete does the other side of the pipe just close does not wait for the rest of the message.
ad8552ef409a3db75335966fb824de490a5ede0e 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793631747 +0100	commit: Fix(Server): Fix the logic and send and receiveData, no check in on errno var to decide what to do next.
64589e03a91e1d095a3dcdb38de67d40a870f696 38404870cb4a2de8ad0288bd7c1e137376be8446 ssoto-su <ssoto-su@student.42malaga.com> 1793797956 +0100	checkout: moving from serg-dev to edu-dev
38404870cb4a2de8ad0288bd7c1e137376be8446 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793797962 +0100	pull --tags origin edu-dev: Fast-forward
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793798070 +0100	checkout: moving from edu-dev to serg-dev
64589e03a91e1d095a3dcdb38de67d40a870f696 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793798421 +0100	checkout: moving from serg-dev to edu-dev
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec 2b3094d3288e2718ecdb758a227a50e9742f9dfd ssoto-su <ssoto-su@student.42malaga.com> 1793798451 +0100	checkout: moving from edu-dev to santy-dev
2b3094d3288e2718ecdb758a227a50e9742f9dfd 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793798971 +0100	checkout: moving from santy-dev to serg-dev
64589e03a91e1d095a3dcdb38de67d40a870f696 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793801554 +0100	checkout: moving from serg-dev to edu-dev
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 ssoto-su <ssoto-su@student.42malaga.com> 1793801634 +0100	checkout: moving from edu-dev to pre-dev
f0ee4e583b1782c153db91f4f6cd7ca1913cc7a4 a4345196abe9a4320deceba96d68ad2b3225bd16 ssoto-su <ssoto-su@student.42malaga.com> 1793801639 +0100	pull --tags origin pre-dev: Fast-forward
a4345196abe9a4320deceba96d68ad2b3225bd16 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793802200 +0100	checkout: moving from pre-dev to edu-dev
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793802384 +0100	checkout: moving from edu-dev to serg-dev
64589e03a91e1d095a3dcdb38de67d40a870f696 967a34b06bfdbf7fa33c2dc4591a01c89594c7ec ssoto-su <ssoto-su@student.42malaga.com> 1793885720 +0100	checkout: moving from serg-dev to edu-dev
967a34b06bfdbf7fa33c2dc4591a01c89594c7ec 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793885783 +0100	checkout: moving from edu-dev to serg-dev
64589e03a91e1d095a3dcdb38de67d40a870f696 2b3094d3288e2718ecdb758a227a50e9742f9dfd ssoto-su <ssoto-su@student.42malaga.com> 1793885861 +0100	checkout: moving from serg-dev to santy-dev
2b3094d3288e2718ecdb758a227a50e9742f9dfd a4345196abe9a4320deceba96d68ad2b3225bd16 ssoto-su <ssoto-su@student.42malaga.com> 1793885881 +0100	checkout: moving from santy-dev to pre-dev
a4345196abe9a4320deceba96d68ad2b3225bd16 1fc2182618796c68c151d3f9f3f965670150a183 ssoto-su <ssoto-su@student.42malaga.com> 1793885908 +0100	checkout: moving from pre-dev to dev
1fc2182618796c68c151d3f9f3f965670150a183 a4345196abe9a4320deceba96d68ad2b3225bd16 ssoto-su <ssoto-su@student.42malaga.com> 1793885936 +0100	checkout: moving from dev to pre-dev
a4345196abe9a4320deceba96d68ad2b3225bd16 1fc2182618796c68c151d3f9f3f965670150a183 ssoto-su <ssoto-su@student.42malaga.com> 1793885953 +0100	checkout: moving from pre-dev to dev
1fc2182618796c68c151d3f9f3f965670150a183 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793885956 +0100	checkout: moving from dev to serg-dev
64589e03a91e1d095a3dcdb38de67d40a870f696 2b3094d3288e2718ecdb758a227a50e9742f9dfd ssoto-su <ssoto-su@student.42malaga.com> 1793886370 +0100	checkout: moving from serg-dev to santy-dev
2b3094d3288e2718ecdb758a227a50e9742f9dfd a4345196abe9a4320deceba96d68ad2b3225bd16 ssoto-su <ssoto-su@student.42malaga.com> 1793886394 +0100	checkout: moving from santy-dev to pre-dev
a4345196abe9a4320deceba96d68ad2b3225bd16 64589e03a91e1d095a3dcdb38de67d40a870f696 ssoto-su <ssoto-su@student.42malaga.com> 1793886405 +0100	checkout: moving from pre-dev to serg-dev
64589e03a91e1d095a3dcdb38de67d40a870f696 9c24aeef03b4731178b0fa0ee095f01bbd3a70b2 ssoto-su <ssoto-su@student.42malaga.com> 1794482945 +0100	pull --tags origin serg-dev: Fast-forward
9c24aeef03b4731178b0fa0ee095f01bbd3a70b2 7886cfd84f9dabd17d3e628be588523325276883 ssoto-su <ssoto-su@student.42malaga.com> 1794495109 +0100	commit: add(Server): New fucn added removeClientFromChannels. when a client quit from server. this function looks for the client in all channel and delete it.
7886cfd84f9dabd17d3e628be588523325276883 50c6a713af66170ce05473e42f04e21d90b9def9 ssoto-su <ssoto-su@student.42malaga.com> 1795011402 +0100	pull --tags origin serg-dev: Fast-forward
50c6a713af66170ce05473e42f04e21d90b9def9 2b3094d3288e2718ecdb758a227a50e9742f9dfd ssoto-su <ssoto-su@student.42malaga.com> 1795011409 +0100	checkout: moving from serg-dev to santy-dev
2b3094d3288e2718ecdb758a227a50e9742f9dfd 48a9fd143030d1873098b876101c6316dc531500 ssoto-su <ssoto-su@student.42malaga.com> 1795011416 +0100	pull --tags origin santy-dev: Fast-forward
48a9fd143030d1873098b876101c6316dc531500 50c6a713af66170ce05473e42f04e21d90b9def9 ssoto-su <ssoto-su@student.42malaga.com> 1795012885 +0100	checkout: moving from santy-dev to serg-dev

```

### Archivo: `./.git/FETCH_HEAD`
```git/FETCH_HEAD
48a9fd143030d1873098b876101c6316dc531500		branch 'santy-dev' of github.com:Santiago-Scheinig/IRC

```

### Archivo: `./.git/COMMIT_EDITMSG`
```git/COMMIT_EDITMSG
add(Server): New fucn added removeClientFromChannels. when a client quit from server. this function looks for the client in all channel and delete it.

```

### Archivo: `./.git/ORIG_HEAD`
```git/ORIG_HEAD
2b3094d3288e2718ecdb758a227a50e9742f9dfd

```

### Archivo: `./.github/workflows/blank.yml`
```yml
# This is a basic workflow to help you get started with Actions

name: CI

# Controls when the workflow will run
on:
  # Triggers the workflow on push or pull request events but only for the "main" branch
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

  # Allows you to run this workflow manually from the Actions tab
  workflow_dispatch:

# A workflow run is made up of one or more jobs that can run sequentially or in parallel
jobs:
  # This workflow contains a single job called "build"
  build:
    # The type of runner that the job will run on
    runs-on: ubuntu-latest

    # Steps represent a sequence of tasks that will be executed as part of the job
    steps:
      # Checks-out your repository under $GITHUB_WORKSPACE, so your job can access it
      - uses: actions/checkout@v4

      # Runs a single command using the runners shell
      - name: Run a one-line script
        run: echo Hello, world!

      # Runs a set of commands using the runners shell
      - name: Run a multi-line script
        run: |
          echo Add other actions to build,
          echo test, and deploy your project.

```

### Archivo: `./include/Channel.hpp`
```hpp
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include <set>
#include <cstddef>

class Client;

class Channel {
	private:
		std::string _name;
		std::map<Client *, bool> _members;
		std::string _topic;
		std::string _key;
		size_t _limit;
		bool _inviteOnly;
		bool _topicRestricted;
		std::set<Client *> _invited;
	public:
		Channel(const std::string &name);
		~Channel();
		const std::string &getName() const;
		void addMember(Client *client, bool isOperator);
		void addOperator(Client *client);
		void removeMember(Client *client);
		bool hasMember(const Client *client) const;
		bool isOperator(const Client *client) const;
		bool isEmpty() const;
		size_t memberCount() const;
};

#endif

```

### Archivo: `./include/Config.hpp`
```hpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:47 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/13 18:10:21 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <cctype>
# include <csignal>
# include <iostream>
# include <sstream>
# include "IRCException.hpp"

extern volatile	sig_atomic_t	g_shutdown;

class Config {
    private:
        int                 _port;
        std::string         _pass;

                            Config();
                            Config(const Config &other);
        Config              &operator=(const Config &other);

        void                parsePort(const std::string &s);
        void                parsePassword(const std::string &s);

		static void         initSignalHandler();
    
    public:
                            Config(int argc, char **argv);
                            ~Config() throw();

        const std::string   &getPassword() const throw();
        int                 getPort() const throw();

};

static void			signalHandler(int signum);

#endif

```

### Archivo: `./include/IRCException.hpp`
```hpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:24:12 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/13 11:43:10 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# define ERROR "\033[31m[ERROR]: "
# define INFO "\033[33m\t[INFO]: "
# define END ".\033[0m\n"

# include <sstream>
# include <stdexcept>
# include <string>

/**
 * SUCCESS,
 * ERR_MEM, Memory allocation error
 * ERR_SCKT, Socket creation error
 * R_INVCMD, Invalid IRC command (send Reply)
 * R_INVNICKNAME, Nickname already in use (send Reply)
 * R_INVCMDARGS, Missing command parameters (send Reply)
 */
enum ircErrco {
	CUSTOM = 0,
	SUCCESS,
	ERR_MEM,
	ERR_SCKT,
	ERR_SCKTOPT,
	ERR_FCNTL,
	ERR_BIND,
	ERR_LISTEN,
	ERR_OPEN,
	ERR_POLL,
	ERR_NEWCL,
	ERR_EXECLIENT,
	ERR_INVCLIENT,
	PARSE_INVALID,
	PARSE_INCOMPLETE,
	R_INVCMD = 421,
	R_INVNICKNAME = 433,
	R_INVCMDARGS = 461
};

/**
 * If errco != 421 && errco != 433 && errco != 461
 * Then its a system error, otherwise its a Reply 
 */
class IRCException : public std::exception {
	
	private:
		std::string			_msg;
		ircErrco			_errco;

	public:
							IRCException(ircErrco errco);
							IRCException(std::string category, std::string msg);
		virtual				~IRCException() throw();

		virtual const char	*what() const throw();
};

class IRCFormatter {

	private:
							IRCFormatter();
							~IRCFormatter();
	
	public:
		static std::string	format(ircErrco errco);
		static std::string	format(std::string category, std::string msg);
};

#endif
```

### Archivo: `./include/Socket.hpp`
```hpp
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <poll.h>
#include "Parser.hpp"

class Socket {

	private:

		const std::string	_ip; 
		std::string			_nickname, _username, _realname;
		bool	 			_hasPass, _registered, _closing;
		
		std::string			_buffIn, _buffOut;
		struct pollfd		_pollFD;
		
							Socket(const Socket &other);
		Socket				&operator=(const Socket &other);

	public:

							Socket(int fd, const std::string &ip);
							~Socket();

		bool				dataRead();
		bool				dataSend();
		bool				nextLine(std::string &line);
		void 				queueOutput(const std::string &msg);
		
		void				setNickname(const std::string &str);
		void				setUsername(const std::string &str);
		void				setRealname(const std::string &str);
		void				setRegistered(bool ans);
		void				setPass(bool ans);
		void				setClosing();
		
		const std::string	&getNickname() const throw();
		const std::string	&getUsername() const throw();
		const std::string	&getRealname() const throw();
		const struct pollfd	&getPollFD() const throw();
		const int			&getFD() const throw();
		const std::string	&getIP() const throw();
		
		bool 				hasPendingOutput() const throw();
		bool				hasPass() const throw();
		bool				isRegistered() const throw();
		bool				isClosing() const throw();
};

#endif

```

### Archivo: `./include/CommandRegistry.hpp`
```hpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:49 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 18:23:50 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

#endif

```

### Archivo: `./include/ICommand.hpp`
```hpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:48 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 18:23:49 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

#endif

```

### Archivo: `./include/Message.hpp`
```hpp
#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>

class Message
{
	/**
	 * All of this private information could instead be a typedef (ex: msgInfo)
	 * This is practical, since we could group and divide pieces of information
	 * regarding how other objects interact with this.
	 * _prefix, _trailing and _hasTrailing could be a typedef (ex: msgInfo)
	 * _cmd and _params could be a typedef (ex: msgParams)
	 * Not necesarilly the case, yet, sometimes, its great to group data since it makes
	 * it easier to travel and identify.
	 */
	private:
		std::string					_prefix;
		std::string					_cmd;
		std::vector<std::string>	_params;
		std::string					_trailing;
		bool						_hasTrailing;

	public:
		Message(const std::string &prefix, const std::string &cmd, const std::vector<std::string> &params, const std::string &trailing, bool hasTrailing);
		~Message();

		const std::string				&getPrefix() const;
		const std::string				&getCommand() const;
		const std::vector<std::string>	&getParams() const;
		const std::string				&getTrailing() const;
		bool							hasTrailing() const;
};

#endif
```

### Archivo: `./include/Parser.hpp`
```hpp
#ifndef PARSER_HPP
# define PARSER_HPP

#include "Message.hpp"
#include <string>
#include <vector>

class Parser
{
	public:
		enum Parse_e			//e_num creado para identifiar cada salida de parser
		{
			PARSE_INCOMPLETE,
			PARSE_MESSAGE,
			PARSE_INVALID
		};

		Parser();
		/**
		* We are missing the other OCF functions, its normative.
		*/
		~Parser();

		//This function should be static, that way i dont have to initialize a Parser object to parse
		//something, likely why Client had a Parser _parser object. Which is redundant.
		//Instead we do:
		// Message processInput(std::string inputBuffer);
		//The parser should then split the information, verify if its correct and return a created
		//Message object with all its information already initialized and ready to use.
		//If something fails, then an exception is thrown, which is catched by the main loop and handled as
		//its supposed to.
		Parse_e processInput(const std::string &data, Message &message);

	private:
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);

		std::string _inputBuffer;

		Parse_e _parseLine(const std::string &line, Message &message) const;
		Parse_e _readPrefix(const std::string &line, std::size_t &pos, std::string &prefix) const;
		Parse_e _readCommand(const std::string &line, std::size_t &pos, std::string &cmd) const;
		Parse_e _readParams(const std::string &line, std::size_t &pos, std::vector<std::string> &params, std::string &trailing, bool &hasTrailing) const;
		void _skipSpaces(const std::string &line, std::size_t &pos) const;
		std::string _toUpper(const std::string &text) const;

};

#endif
```

### Archivo: `./include/Replies.hpp`
```hpp
#ifndef REPLIES_HPP
# define REPLIES_HPP

/**
 * Should use IRCErrcoFormatter to generate Replies with the respective
 * generic format.
 */

 #include <string>

class Replies
{
	private:
		Replies();
		Replies(const Replies &other);
		Replies &operator=(const Replies &other);

public:
		// CAPABILITIES: Capability negotiation (CAP LS, LIST, ACK, NAK)
		static std::string	capLs(const std::string &target, const std::string &capabilities);
		static std::string	capList(const std::string &target, const std::string &capabilities);
		static std::string	capAck(const std::string &target, const std::string &capabilities);
		static std::string	capNak(const std::string &target, const std::string &capabilities);
		// CONNECTION: Connection and client registration
		static std::string	pong(const std::string &serverName, const std::string &token);
		static std::string	welcome(const std::string &serverName, const std::string &nickname);
		// NICKNAME
		static std::string	nick(const std::string &prefix, const std::string &nickname);
		// CHANNEL MEMBERSHIP: Joining, leaving and listing channel members.
		static std::string	join(const std::string &prefix, const std::string &channel);
		static std::string	namesReply(const std::string &serverName, const std::string &target, const std::string &channel, const std::string &names);
		static std::string	endOfNames(const std::string &serverName, const std::string &target, const std::string &channel);
		static std::string	part(const std::string &prefix, const std::string &channel, const std::string &reason);
		static std::string	quit(const std::string &prefix, const std::string &reason);
		// MESSAGING: Private messages between clients or channels.
		static std::string	privmsg(const std::string &prefix, const std::string &target, const std::string &text);
		// TOPIC: Channel topic changes and queries.
		static std::string	topic(const std::string &prefix, const std::string &channel, const std::string &text);
		static std::string	topicReply(const std::string &serverName, const std::string &target, const std::string &channel, const std::string &topic);
		static std::string	noTopic(const std::string &serverName, const std::string &target, const std::string &channel);
		// CHANNEL MANAGEMENT: Invitations, kicks and mode changes.
		static std::string	invite(const std::string &prefix, const std::string &nickname, const std::string &channel);
		static std::string	kick(const std::string &prefix, const std::string &channel, const std::string &nickname, const std::string &reason);
		static std::string	mode(const std::string &prefix, const std::string &target, const std::string &modes, const std::string &parameters);
		// NUMERIC REPLIES: Standard IRC numeric responses and error messages.
		static std::string	numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text);
};

#endif

```

### Archivo: `./include/Server.hpp`
```hpp
#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "IRCException.hpp"
# include "Socket.hpp"

#	include <vector>
#	include <map>
#	include <cerrno>
#	include <cstring>
#	include <fstream>
#	include <poll.h>
#	include <fcntl.h>
#	include <unistd.h>
#	include <sys/socket.h>
#	include <sys/types.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>

class Server {
	private:

		const int 				_port;
		const std::string		_password;

		std::map<int , Socket*>	_clientArr;
		struct pollfd			_pollFD;
		int						_spareFD;

								Server(const Server &copy);
								Server& operator=(const Server &copy);

		int 					initSocket() const;
		int 					initAddr(const int serverFD) const;
		void					initFDS(const int serverFD);

		void 					addNewClient();
		int						acceptClient(struct sockaddr_in &address);
		void 					manageClient(std::map<int, Socket*>::iterator client_it, short revents);
		void 					deleteClient(std::map<int, Socket*>::iterator client_it);
		
		void 					reapClosing();
	
	public:
	
								Server(Config init);
								~Server();

		void 					run();
		bool 					matchPassword(const std::string &pass) const throw();
};

#endif

```

### Archivo: `./src/Channel.cpp`
```cpp
#include "Channel.hpp"

Channel::Channel(const std::string &name) 
    : _name(name),
    _topic(""), 
    _key(""), 
    _limit(0), 
    _inviteOnly(false), 
    _topicRestricted(false) {
}

Channel::~Channel() {}

const std::string &Channel::getName() const {
    return _name;
}

void Channel::addMember(Client *client, bool isOperator) {
    if (client == NULL)
        return;
    std::map<Client *, bool>::iterator it = _members.find(client);
    if (it == _members.end())
        _members.insert(std::make_pair(client, isOperator));
    _invited.erase(client);
}

void Channel::addOperator(Client *client) {
    std::map<Client *, bool>::iterator it = _members.find(client);
    if (it == _members.end())
        return;
    it->second = true;
}

void Channel::removeMember(Client *client) {
    _members.erase(client);
    _invited.erase(client);
}

bool Channel::hasMember(const Client *client) const {
    return _members.find(const_cast<Client*>(client)) != _members.end();
}

bool Channel::isOperator(const Client *client) const {
    std::map<Client *, bool>::const_iterator it = _members.find(const_cast<Client*>(client));
    if (it == _members.end())
        return false;
    return it->second;
}

bool Channel::isEmpty() const {
    return _members.empty();
}

size_t Channel::memberCount() const {
    return _members.size();
}
```

### Archivo: `./src/Config.cpp`
```cpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:30 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/13 18:10:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

                    Config::Config() {
    throw IRCException::IRCException("INV_CONSTRUCTOR", "Cant use default constructor for Config class");
}

                    Config::Config(int argc, char **argv) {
    if (argc != 3)
        throw IRCException::IRCException("INV_CONFIG", "usage: ./ircserv <port> <password>");
    parsePort(argv[1]);
    parsePassword(argv[2]);
    initSignalHandler();
}

                    Config::Config(const Config &other){
    *this = other;
}

Config              &Config::operator=(const Config &other){
    if (this != &other) {
        _port = other._port;
        _pass = other._pass;
    }
    return (*this);
}

                    Config::~Config() throw() {}

void                Config::parsePort(const std::string &s) {
    int size = s.size();

    if (s.empty())
        throw IRCException::IRCException("inv_port", "port is empty");
    if (size > 5)
        throw IRCException::IRCException("inv_port", "port values: [1 - 65535]");
    for (std::size_t i = 0; i < size; ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            throw IRCException::IRCException("inv_port", "non numeric character found");
    
    std::stringstream   stream(s);

    if (!(stream >> _port) || !stream.eof() || _port < 1 || _port > 65535)
        throw IRCException::IRCException("inv_port", "port values: [1 - 65535]");
}

void                Config::parsePassword(const std::string &s) {
    int size = s.size();
    
    if (s.empty())
        throw IRCException::IRCException("inv_password", "password cannot be empty");
    for (std::size_t i = 0; i < size; i++)
        if (!std::isprint(static_cast<unsigned char>(s[i])))
            throw IRCException::IRCException("inv_password", "password cannot have non printable characters");
	_pass = s;
}

void			signalHandler(int signum) {
	if (signum == SIGINT)
		g_shutdown = 1;
}

void                Config::initSignalHandler() {
	struct sigaction	sa;
	struct sigaction	sa_pipe;

	std::memset(&sa_pipe, 0, sizeof(sa_pipe));
	sa_pipe.handler = SIG_IGN;
	sigemptyset(&sa_pipe.sa_mask);
	if (sigaction(SIGPIPE, &sa_pipe, NULL) == 1) {
		throw std::exception(); //Failed to ignore SIGPIPE
	}
	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == 1) {
		throw std::exception(); //Failed to register SIGINT handler
	}
}

const std::string   &Config::getPassword() const throw() {
    return (_pass);
}

int                 Config::getPort() const throw() {
    return (_port);
}

```

### Archivo: `./src/IRCException.cpp`
```cpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:21 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 19:23:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCException.hpp"

			IRCException::IRCException(ircErrco errco)
				:	_errco(errco),
					_msg(IRCFormatter::format(errco)) {}

			IRCException::IRCException(std::string category, std::string msg)
				:	_errco(CUSTOM),
					_msg(IRCFormatter::format(category, msg)) {}

			IRCException::~IRCException() throw() {}

const char	*IRCException::what() const throw() {
	return (_msg.c_str());
}

std::string	IRCFormatter::format(ircErrco errco) {
	std::stringstream	ss;
//Should? add a break in every case?
	switch (errco) {
		case SUCCESS:
			ss << INFO << "Success!" << END;
		case ERR_MEM:
			ss << ERROR << "[ERR_MEM]" << END << INFO << "Memory allocation failed" << END;
		case ERR_SCKT:
			ss << ERROR << "[ERR_SCKT]" << END << INFO << "Socket creation failed" << END;
		case R_INVCMD:
			ss << ERROR << "[R_INVCMD]" << END << INFO << "Unknown command" << END;
		case R_INVNICKNAME:
			ss << ERROR << "[R_INVNICKNAME]" << END << INFO << "Nickname already in use" << END;
		case R_INVCMDARGS:
			ss << ERROR << "[R_INVCMDARGS]" << END << INFO << "Missing/Invalid command parameters" << END;
		default:
			ss << ERROR << "[UNKNOWN]" << END << INFO << "Unrecognized error code" << END;
	}
	return (ss.str());
}

std::string	IRCFormatter::format(std::string category, std::string msg) {
	std::stringstream	ss;
	std::string upperCase = category;

	for (std::size_t i = 0; i < upperCase.size(); ++i)
		upperCase[i] = std::toupper(upperCase[i]);
	ss << ERROR << "[" << upperCase << "]" << END << INFO << msg << END;
	return (ss.str());
}
```

### Archivo: `./src/Parser.cpp`
```cpp
#include "../include/Parser.hpp"

#include <cctype>

Parser::Parser() : _inputBuffer(){}

Parser::~Parser() {}

/**
 * processInput should return a Message object instead of taking its reference.
 */
Parser::Parse_e Parser::processInput(const std::string &input, Message &message)
{
	std::size_t	end;
	std::string	line;

	_inputBuffer += input;				// haya o no haya input rabajamos en inputbuffer
	end = _inputBuffer.find("\r\n");
	// Sin CRLF aun no hay linea completa: conservamos el buffer.
	if (end == std::string::npos)
		return PARSE_INCOMPLETE;
	line = _inputBuffer.substr(0, end);
	_inputBuffer.erase(0, end + 2);
	// La linea existe, pero no contiene ningun comando que parsear.
	if (line.empty())
		return PARSE_INVALID; //throw IRCException::IRCException(PARSE_INVALID);
	return _parseLine(line, message);		// devolvemos true y message struct.
}

void Parser::_skipSpaces(const std::string &line, std::size_t &pos) const
{
	while (pos < line.size() && line[pos] == ' ')
		pos++;
}

/**
 * We can use exceptions, and turn this function into a void return, specially since is const.
 */
Parser::Parse_e Parser::_readPrefix(const std::string &line, std::size_t &pos, std::string &prefix) const
{
	std::size_t	space;

	if (pos >= line.size() || line[pos] != ':')
		return PARSE_MESSAGE;
	space = line.find(' ', pos);
	if (space == std::string::npos || space == pos + 1) //prefix debe ser :prefix
		return PARSE_INVALID; //std::error prefijo invalido
	prefix = line.substr(pos + 1, space - pos - 1);
	pos = space + 1;									// actualizamos pos al inicio de sigiente palabra
	return PARSE_MESSAGE;
}

/**
 * We can use exceptions, and turn this function into a void return, specially since is const.
 */
Parser::Parse_e Parser::_readCommand(const std::string &line, std::size_t &pos, std::string &cmd) const
{
	std::size_t	space;

	_skipSpaces(line, pos);
	if (pos >= line.size() || line[pos] == ':')
		return PARSE_INVALID; // despues del prefijo debe existir un comando.
	space = line.find(' ', pos);
	if (space == std::string::npos)
		cmd = line.substr(pos);
	else
	{
		cmd = line.substr(pos, space - pos);
		pos = space + 1;
	}
	if (cmd.empty())
		return PARSE_INVALID; // no podemos construir un Message sin comando.
	cmd = _toUpper(cmd);
	return PARSE_MESSAGE;
}

/**
 * We can use exceptions, and turn this function into a void return, specially since is const.
 */
Parser::Parse_e Parser::_readParams(const std::string &line, std::size_t &pos, std::vector<std::string> &params, std::string &trailing, bool &hasTrailing) const
{
	std::size_t	space;

	while (pos < line.size())
	{
		_skipSpaces(line, pos);
		if (pos >= line.size())
			break;
		if (line[pos] == ':')
		{
			trailing = line.substr(pos + 1);
			hasTrailing = true;
			return PARSE_MESSAGE; // ':' consume todo el resto como trailing.
		}
		space = line.find(' ', pos);
		if (space == std::string::npos)
		{
			params.push_back(line.substr(pos));
			break;
		}
		params.push_back(line.substr(pos, space - pos));
		pos = space + 1;
	}
	return PARSE_MESSAGE; // cero parametros tambien puede ser sintacticamente valido.
}

/**
 * We can use exceptions, and turn this function into a void return, specially since is const.
 */
Parser::Parse_e Parser::_parseLine(const std::string &line, Message &message) const
{
	std::size_t	pos = 0;		//en cada funcion actualizamos pos para mantener el index en line actualizado correctamente
	std::string	prefix;
	std::string	command;
	std::vector<std::string> params;
	std::string	trailing;
	bool		hasTrailing = false;

	if (line.empty())
		return PARSE_INVALID; // CRLF sin contenido: linea invalida.
	if (_readPrefix(line, pos, prefix) == PARSE_INVALID)
		return PARSE_INVALID;
	if (_readCommand(line, pos, command) == PARSE_INVALID)
		return PARSE_INVALID;
	if (_readParams(line, pos, params, trailing, hasTrailing) == PARSE_INVALID)
		return PARSE_INVALID;
	message = Message(prefix, command, params, trailing, hasTrailing);
	return PARSE_MESSAGE;
}

/**
 * Pretty sure there's already a function std:: that does this.
 */
std::string Parser::_toUpper(const std::string &cmd) const
{
	std::string tmp = cmd;

	for (std::size_t i = 0; i < tmp.size(); ++i)
		tmp[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(tmp[i])));
	return tmp;
}

```

### Archivo: `./src/Server.cpp`
```cpp
#include "Server.hpp"

/* ------------------------------------------------------------------------ */

/* ---------------------------   CONSTRUCTORS   --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
		Server::Server(Config init)
					:	_port(init.getPort()), _password(init.getPassword()), _spareFD(-1) {
	initFDS(initAddr(initSocket()));
}

/**
 * 
 */
int		Server::initSocket() const {
	int serverFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFD < 0)
		throw std::runtime_error(IRCFormatter::format(ERR_SCKT));
	
	int	opt = 1;
	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_SCKTOPT));
	}
	
	if (fcntl(serverFD, F_SETFL, O_NONBLOCK) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_FCNTL));
	}

	return (serverFD);
}

/**
 * 
 */
int		Server::initAddr(const int serverFD) const {
	struct sockaddr_in	address;
	
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	
	if (bind(serverFD, (struct sockaddr *) &address, sizeof(address)) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_BIND));
	}
	
	if (listen(serverFD, SOMAXCONN) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_LISTEN));
	}

	return (serverFD);
}

/**
 * 
 */
void	Server::initFDS(const int serverFD) {
	_pollFD.fd = serverFD;
	_pollFD.events = POLLIN;
	_pollFD.revents = 0;

	_spareFD = open("/dev/null", O_RDONLY);
	if (_spareFD < 0) {
		close(_pollFD.fd);
		throw std::runtime_error(IRCFormatter::format(ERR_OPEN));
	}
}

/**
 * 
 */
		Server::~Server() {
	while (!_clientArr.empty())
		deleteClient(_clientArr.begin());
	close(_pollFD.fd);
	close(_spareFD);
}

/* ------------------------------------------------------------------------ */

/* ----------------------      SERVER MANAGMENT      ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
void	Server::run() {

	while (!g_shutdown) {
		std::vector<struct pollfd> pollArr;

		pollArr.push_back(_pollFD);
		for (std::map<int, Socket *>::iterator client_it = _clientArr.begin(); client_it != _clientArr.end(); ++client_it)
			pollArr.push_back(client_it->second->getPollFD());
		
		if (poll(&pollArr[0], pollArr.size(), -1) < 0) {
			if (errno == EINTR) {
				if (g_shutdown)
					break;
				continue;
			}
			throw std::runtime_error(IRCFormatter::format(ERR_POLL));
		}
		
		if (pollArr[0].revents & (POLLERR | POLLNVAL))
			throw std::runtime_error(IRCFormatter::format(ERR_LISTEN));
		
		try {
			if (pollArr[0].revents & POLLIN)
				addNewClient();
		}
		catch (IRCException::IRCException &e) {
			//Handle failed client acceptance.
		}	
		
		for (size_t i = 1; i < pollArr.size(); ++i) {
			
			if (g_shutdown)
				break;
			
			if (!pollArr[i].revents)
				continue;
			
			std::map <int, Socket *>::iterator client_it = _clientArr.find(pollArr[i].fd);
	
			if (client_it == _clientArr.end())
				continue;
				
			try {
				manageClient(client_it, pollArr[i].revents);
			}
			catch (const IRCException::IRCException &e) {
				//Handle client specific errors.
				client_it = _clientArr.find(pollArr[i].fd);
				if (client_it != _clientArr.end())
					deleteClient(client_it);
			}
		}
		reapClosing();
	}	
}

/**
 * 
 */
bool	Server::matchPassword(const std::string &pass) const throw() {
	return (_password == pass);
}

/**
 * 
 */
void	Server::reapClosing() {
	typedef std::map<int, Socket*>::iterator Socket_it;
	
	for (Socket_it pos = _clientArr.begin(); pos != _clientArr.end(); ;) {
		Socket_it current_pos = pos++;
		if (current_pos->second->isClosing() && !current_pos->second->hasPendingOutput()) {
			deleteClient(current_pos);
		}
	}
}

/* ------------------------------------------------------------------------ */

/* ----------------------      CLIENT MANAGMENT      ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
void	Server::addNewClient() {
	struct sockaddr_in	address;

	int fd = acceptClient(address);

	Socket *client = NULL;
    try {
        client = new Socket(fd, std::string(inet_ntoa(address.sin_addr)));
        if (!_clientArr.insert(std::make_pair(fd, client)).second)
            throw std::runtime_error(IRCFormatter::format(ERR_NEWCL));
        client = NULL;
    }
    catch (...) {
        delete client;
        close(fd);
        throw; //Throw a formatted message using the errno.
    }
}

/**
 * 
 */
int		Server::acceptClient(struct sockaddr_in &address) {
    socklen_t addrlen = sizeof(address);

    int fd = accept(_pollFD.fd, (struct sockaddr *)&address, &addrlen);
    if (fd < 0) {
        if (errno == EMFILE || errno == ENFILE) {
            close(_spareFD);
            _spareFD = -1;
            int tmp = accept(_pollFD.fd, NULL, NULL);
            if (tmp >= 0) {
                const char msg[] = "ERROR: server full\r\n"; //Needs reply formatting.
                send(tmp, msg, sizeof(msg) - 1, MSG_NOSIGNAL);
                close(tmp);
            }
            _spareFD = open("/dev/null", O_RDONLY);
        }
		//Where is this constructor coming from?
        throw IRCException::IRCException(ERR_INVCLIENT, "failed to accept client address");
    }
	
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        close(fd);
		//its here also!
        throw IRCException::IRCException(ERR_INVCLIENT, "failed to initialize client [fcntl()]");
    }

	return (fd);
}

/**
 * 
 */
void	Server::manageClient(std::map<int, Socket*>::iterator client_it, short revents) {
	std::string	line;

	Socket	*client = client_it->second;
	bool	alive = true;
	
	//Invalid descriptor: remove immediatly.
	if (revents & POLLNVAL) {
		deleteClient(client_it);
		return;
	}
	
	//Read avalible data. Process every complete line in the receive buffer.
	if (revents & (POLLIN | POLLHUP | POLLERR) && !client->isClosing()) {
		alive = client->dataRead();
		while (alive && !client->isClosing() && client->nextLine(line)) {
			if (line.empty())
				continue;
			try {
				Message cmd(Parser::) //What is this?
				CommandRegistry::exe(Parser::processInput(line), *this, *client);
			}
			catch(const IRCException::IRCexception &e) {
				//Handle invalid IRC message.
					//Build a reply of invalid input. Fill the client _buffOut with it.
				//Continue processing subsequent complete lines.
			}
		}
	}
	
	//Flush pending output.
	if (alive && (revents & POLLOUT))
		alive = client->dataSend();
	
	//Remove dead or disconnected clients.
	if (!alive || (revents & (POLLHUP | POLLERR)))
		deleteClient(client_it);
}

/**
 * 
 */
void	Server::deleteClient(std::map<int, Socket*>::iterator client_it) {
	close(client_it->first);
	delete client_it->second;
	_clientArr.erase(client_it);
}

/* ------------------------------------------------------------------------ */

/* ---------------------------        END       --------------------------- */

/* ------------------------------------------------------------------------ */
```

### Archivo: `./src/Socket.cpp`
```cpp
#include "Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <string>


/* ------------------------------------------------------------------------ */

/* ---------------------------   CONSTRUCTORS   --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
		Socket::Socket(int fd, const std::string &ip)	
			:	_ip(ip), _hasPass(false), _registered(false), _closing(false) {
	_pollFD.fd = fd;
	_pollFD.events = POLLIN;
	_pollFD.revents = 0;
}

/**
 * 
 */
		Socket::~Socket() {}


/* ------------------------------------------------------------------------ */

/* --------------------------- MEMBER FUNCTIONS --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
bool	Socket::dataRead() {
	char	buf[512];
	ssize_t n = recv(_pollFD.fd, buf, sizeof(buf), 0);

	if (n <= 0) {
		return (false);
	}
	_buffIn.append(buf, n);
	return (true);
}

/**
 * 
 */
bool	Socket::nextLine(std::string &line) {
	std::string::size_type pos = _buffIn.find('\n');

	if (pos == std::string::npos) {
		if (_buffIn.size() > 512)	//No terminator and already too long
			_buffIn.clear();		// or queue ERR_INPUTTOOLONG (417)
		return (false);
	}
	line = _buffIn.substr(0, pos);
	_buffIn.erase(0, pos + 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (true);
}

/**
 * 
 */
void	Socket::queueOutput(const std::string &msg) {
	_buffOut += msg;
	_pollFD.events |= POLLOUT;
}

/**
 * 
 */
bool	Socket::dataSend() {
	if (!_buffOut.empty()) {
		ssize_t n = send(_pollFD.fd, _buffOut.data(), _buffOut.size(), MSG_NOSIGNAL);
		if (n <= 0) {
			return (false);
		}
		_buffOut.erase(0, n);
	}
	if (_buffOut.empty())
		_pollFD.events &= ~POLLOUT;
	return (true);
}

/* ------------------------------------------------------------------------ */

/* ---------------------------      SETTERS     --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
void	Socket::setNickname(const std::string &str) {
	_nickname = str;
}

/**
 * 
 */
void	Socket::setUsername(const std::string &str) {
	_username = str;
}

/**
 * 
 */
void	Socket::setRealname(const std::string &str) {
	_realname = str;
}

/**
 * 
 */
void	Socket::setRegistered(bool ans) {
	_registered = ans;
}

/**
 * 
 */
void	Socket::setPass(bool ans) {
	_hasPass = ans;
}

/**
 * 
 */
void	Socket::setClosing() {
	_closing = true;
	_pollFD.events &= ~POLLIN;
}

/* ------------------------------------------------------------------------ */

/* ---------------------------      GETTERS     --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
const std::string		&Socket::getNickname() const throw() {
	return (_nickname);
}

/**
 * 
 */
const std::string		&Socket::getUsername() const throw() {
	return (_username);
}

/**
 * 
 */
const std::string		&Socket::getRealname() const throw() {
	return (_realname);
}

/**
 * 
 */
const struct pollfd		&Socket::getPollFD() const throw() {
	return (_pollFD);
}

/**
 * 
 */
const int				&Socket::getFD() const throw() {
	return (_pollFD.fd);
}

/**
 * 
 */
const std::string		&Socket::getIP() const throw() {
	return (_ip);
}

/**
 * 
 */
bool					Socket::hasPendingOutput() const throw() {
	return !(_buffOut.empty());
}

/**
 * 
 */
bool					Socket::hasPass() const throw() {
	return (_hasPass);
}

/**
 * 
 */
bool					Socket::isRegistered() const throw() {
	return (_registered);
}

/**
 * 
 */
bool					Socket::isClosing() const throw() {
	return (_closing);
}

/* ------------------------------------------------------------------------ */

/* ---------------------------        END       --------------------------- */

/* ------------------------------------------------------------------------ */

```

### Archivo: `./src/CommandRegistry.cpp`
```cpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandRegistry.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:36 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 19:01:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



```

### Archivo: `./src/ICommand.cpp`
```cpp
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:34 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 18:23:35 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



```

### Archivo: `./src/Message.cpp`
```cpp
#include "../include/Message.hpp"

Message::Message(const std::string &prefix, const std::string &cmd, const std::vector<std::string> &params, const std::string &trailing, bool hasTrailing)	:	_prefix(prefix),
																																								_cmd(cmd),
																																								_params(params),
																																								_trailing(trailing),
																																								_hasTrailing(hasTrailing) {}

Message::~Message() {}

const std::string &Message::getPrefix() const {
	return (_prefix);
}

const std::string &Message::getCommand() const {
	return (_cmd);
}

const std::vector<std::string> &Message::getParams() const {
	return (_params);
}

const std::string &Message::getTrailing() const {
	return (_trailing);
}

bool Message::hasTrailing() const {
	return (_hasTrailing);
}

```

### Archivo: `./src/Replies.cpp`
```cpp
#include "../include/Replies.hpp"

Replies::Replies(){}
// REGISTER REPLIES
std::string Replies::capLs(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " LS :" + capabilities + "\r\n";
}
std::string Replies::capList(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " LIST :" + capabilities + "\r\n";
}
std::string Replies::capAck(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " ACK :" + capabilities + "\r\n";
}
std::string Replies::capNak(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " NAK :" + capabilities + "\r\n";
}
std::string Replies::nick(const std::string &prefix, const std::string &nickname)
{
	return ":" + prefix + " NICK " + nickname + "\r\n";
}

std::string Replies::welcome(const std::string &serverName, const std::string &nickname)
{
	return numeric(serverName, "001", nickname, "Welcome to IRC server");
}

// MESSAGE REPLIES
std::string Replies::pong(const std::string &serverName, const std::string &token)
{
	return ":" + serverName + " PONG " + serverName + " :" + token + "\r\n";
}

std::string Replies::privmsg(const std::string &prefix, const std::string &target, const std::string &text)
{
	return ":" + prefix + " PRIVMSG " + target + " :"
		+ text + "\r\n";
}

std::string Replies::quit(const std::string &prefix, const std::string &reason)
{
	return ":" + prefix + " QUIT :" + reason + "\r\n";
}

// CHANNELS REPLIES
std::string Replies::join(const std::string &prefix, const std::string &channel)
{
	return ":" + prefix + " JOIN :" + channel + "\r\n";
}

std::string Replies::namesReply(const std::string &serverName, const std::string &target, const std::string &channel, const std::string &names)
{
	return ":" + serverName + " 353 " + target + " = " + channel + " :" + names + "\r\n";
}

std::string Replies::endOfNames(const std::string &serverName, const std::string &target, const std::string &channel)
{
	return ":" + serverName + " 366 " + target + " " + channel + " :End of /NAMES list\r\n";
}

std::string Replies::part(const std::string &prefix, const std::string &channel, const std::string &reason)
{
	std::string reply = ":" + prefix + " PART " + channel;

	if (!reason.empty())
		reply += " :" + reason;
	return reply + "\r\n";
}

std::string Replies::topic(const std::string &prefix, const std::string &channel, const std::string &text)
{
	return ":" + prefix + " TOPIC " + channel + " :" + text + "\r\n";
}
std::string Replies::topicReply(const std::string &serverName, const std::string &target, const std::string &channel, const std::string &topic)
{
	return ":" + serverName + " 332 " + target + " " + channel + " :" + topic + "\r\n";
}

std::string Replies::noTopic(const std::string &serverName, const std::string &target, const std::string &channel)
{
	return ":" + serverName + " 331 " + target + " " + channel + " :No topic is set\r\n";
}

std::string Replies::invite(const std::string &prefix, const std::string &nickname, const std::string &channel)
{
	return ":" + prefix + " INVITE " + nickname + " " + channel + "\r\n";
}

std::string Replies::kick(const std::string &prefix, const std::string &channel, const std::string &nickname, const std::string &reason)
{
	std::string reply = ":" + prefix + " KICK " + channel + " " + nickname;

	if (!reason.empty())
		reply += " :" + reason;
	return reply + "\r\n";
}

std::string Replies::mode(const std::string &prefix, const std::string &target, const std::string &modes, const std::string &parameters)
{
	std::string reply = ":" + prefix + " MODE " + target + " " + modes;

	if (!parameters.empty())
		reply += " " + parameters;
	return reply + "\r\n";
}

// NUMERIC ERROR REPLIES
std::string Replies::numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text)
{
	return ":" + serverName + " " + code + " " + target + " :" + text + "\r\n";
}

/* comandos esperados:

CAP
INVITE
JOIN
KICK
MODE
NICK
PART
PASS
PING
PRIVMSG
QUIT
TOPIC
USER
*/
```

### Archivo: `./main.cpp`
```cpp
#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <exception>

volatile sig_atomic_t	g_shutdown = 0;

int	main(int argc, char** argv)
{
	try
	{
		Config	dflt(argc, argv);
		Server	server(dflt);
		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ircserv: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}

```

### Archivo: `./Makefile`
```/Makefile
NAME		:= ircserv

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -Iinclude

SRCS		:= main.cpp \
			   src/Server.cpp \
			   src/Socket.cpp \
			   src/Channel.cpp \
			   src/Message.cpp \
			   src/Parser.cpp \
			   src/CommandRegistry.cpp \
			   src/ICommand.cpp \
			   src/Replies.cpp \
			   src/Config.cpp \
			   src/IRCException.cpp
OBJS		:= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

```

