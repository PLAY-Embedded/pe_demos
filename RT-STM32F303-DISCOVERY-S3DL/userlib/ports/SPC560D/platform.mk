PLATFORMPATH = $(USERLIB)/platforms/SPC560D
# List of all the SPC560D platform files.
S3DLPLATFORMSRC = $(PLATFORMPATH)/3DVIDEO/CHIBICUBEv2/draw_lld.c \
                  $(PLATFORMPATH)/3DVIDEO/CHIBICUBEv2/video3d_lld.c \
                  $(PLATFORMPATH)/CONTROLLER/controller_lld.c \
                  $(PLATFORMPATH)/CONTROLLER/serial_driver_config.c \
                  $(PLATFORMPATH)/CONTROLLER/SHELL/shell_lld.c

                               
# Required include directories
S3DLPLATFORMINC = $(PLATFORMPATH)/3DVIDEO/CHIBICUBEv2 \
                  $(PLATFORMPATH)/CONTROLLER \
                  $(PLATFORMPATH)/CONTROLLER/SHELL \
                  $(PLATFORMPATH)/MEMS