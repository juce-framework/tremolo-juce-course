#!/bin/zsh

PLUGIN_PATH=$1
PLUGIN_FILE_NAME=$PLUGIN_PATH:t
echo "Creating an installer for $PLUGIN_FILE_NAME"

DEST_DIR="/Library/Audio/Plug-Ins/VST3"

SCRIPTS_PATH="package_scripts"
POSTINSTALL_SCRIPT_PATH="$SCRIPTS_PATH/postinstall"
mkdir $SCRIPTS_PATH

echo "#!/bin/zsh" > $POSTINSTALL_SCRIPT_PATH
echo "xattr -cr $DEST_DIR/$PLUGIN_FILE_NAME\n" >> $POSTINSTALL_SCRIPT_PATH

chmod +x $POSTINSTALL_SCRIPT_PATH

pkgbuild --scripts $SCRIPTS_PATH --install-location $DEST_DIR --component $PLUGIN_PATH "$PLUGIN_FILE_NAME.pkg"

