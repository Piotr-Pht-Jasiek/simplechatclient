#!/bin/bash
cp /usr/bin/scc ./pkg-debian/usr/bin/scc
cp -r /usr/share/scc/* ./pkg-debian/usr/share/scc/

chmod 755 ./pkg-debian/usr/bin/scc
find ./pkg-debian/usr/share/scc/ -type d -exec chmod 755 {} \;
find ./pkg-debian/usr/share/scc/ -type f -exec chmod 644 {} \;

cd pkg-debian
find . -type f ! -regex '.*\.hg.*' ! -regex '.*?debian-binary.*' ! -regex '.*?DEBIAN.*' -printf '%P ' | xargs md5sum > DEBIAN/md5sums 
cd ..

fakeroot dpkg -b pkg-debian/ scc-1.8.0.0.deb
fakeroot alien -c -k -r scc-1.8.0.0.deb
