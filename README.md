## GnuSocialShell

GnuSocialShell is a simple, userful, text-based GnuSocial client write in C by DalmeGNU to all UNIX-like Operating Systems

### How to compile GnuSocialShell

#### Dependencies

- libcurl-dev


#### Compile
      $ touch config.rpath
      $ autoreconf -iv
      $ ./configure --prefix=/usr/local
      $ make

#### Execute
      $ ./gnusocialshell

### How to make a Configuration File

First we have to open the file "config" using nano (or another text editor)

      $ nano config

#### We will get something like:
      #Default Config file for GnuSocialShell
      protocol=;
      user=;
      server=;
      #Uncomment this line to enable password store
      #password=;

Now we have to complete protocol, user, server and if we want password.
- Protocol should always be "https", but "http" will work
- User should be our screen name, so if my account is @account@mynode.com, user should be "account"
- Server should be our node URL, so "mynode.com"
- If we want we can store our password in the config file but we have to uncomment the line. If not GnuSocialShell will ask we the password.

#### Example
      #Default Config file for GnuSocialShell
      protocol=https;
      user=account;
      server=mynode.com;
      #Uncomment this line to enable password store
      password=123mypassword;
