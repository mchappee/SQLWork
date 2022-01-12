Summary: SqlWork is a SQL builder/executer for Oracle databases.
Name: sqlwork
Version: 5.0
Release: pre3
Copyright: GPL
Group: Applications/Databases
Source: ftp://oracle.mattshouse.com/pub/sqlwork-%{version}-%{release}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
SQLWork is a visual SQL (Structured Query Language) input and retrieval
program. It is useful for both DBAs and database developers. Query Results can
be displayed in plain text or in grid form and (where supported by the
database) query plans may be viewed as an aid to database performance tuning.

SQLWork is the companion tool to Orasoft's Object Manager a graphical database
navigation and administration tool. Both tools are released under the GNU
General Public License.

Commercial support for these tools is available from Advantio - visit
www.advantio.com for details

%prep
%setup -q

%build
config_opts=`rpm -q MySQL-devel >/dev/null 2>&1 || echo --disable-mysql`
config_opts="$config_opts "`if [ -z "${ORACLE_HOME}" ]; then echo --disable-oracle; fi`
./configure --prefix=/usr $config_opts
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
make prefix=$RPM_BUILD_ROOT/usr install

%files
%defattr(-,root,root)
/usr/bin/sqlwork
/usr/share/sqlwork/pixmaps/*
%doc README TODO COPYING ChangeLog

#%clean
#rm -rf $RPM_BUILD_ROOT
