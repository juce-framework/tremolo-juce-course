#!/bin/zsh

set -o xtrace

PLUGIN_PATH=$1
PLUGIN_FILE_NAME=$PLUGIN_PATH:t
echo "Creating an installer for $PLUGIN_FILE_NAME"

DEST_DIR="/Library/Audio/Plug-Ins/VST3"

SCRIPTS_PATH="package_scripts"
POSTINSTALL_SCRIPTS_PATH="$SCRIPTS_PATH/postinstall"
mkdir -p $POSTINSTALL_SCRIPTS_PATH
POSTINSTALL_SCRIPT_PATH="$POSTINSTALL_SCRIPTS_PATH/xattr_plugin.sh"

echo "#!/bin/zsh" > $POSTINSTALL_SCRIPT_PATH
echo "set -o xtrace" >> $POSTINSTALL_SCRIPT_PATH
echo "echo Here >> /Users/jawi/Desktop/debug.txt" >> $POSTINSTALL_SCRIPT_PATH
echo "sudo xattr -cr $DEST_DIR/$PLUGIN_FILE_NAME" >> $POSTINSTALL_SCRIPT_PATH
echo "set +o xtrace" >> $POSTINSTALL_SCRIPT_PATH

# pkgbuild --scripts $SCRIPTS_PATH --install-location $DEST_DIR --root $PLUGIN_PATH --component-plist "$PLUGIN_PATH/Contents/Info.plist" "$PLUGIN_FILE_NAME.pkg"
# pkgbuild --scripts $SCRIPTS_PATH --install-location $DEST_DIR --component $PLUGIN_PATH "$PLUGIN_FILE_NAME.pkg"
pkgbuild --install-location $DEST_DIR --component $PLUGIN_PATH "$PLUGIN_FILE_NAME.pkg"

set +o xtrace
