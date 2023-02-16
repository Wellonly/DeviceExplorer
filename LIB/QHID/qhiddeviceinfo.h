#ifndef QHIDDEVICEINFO_H
#define QHIDDEVICEINFO_H

#include <QString>

struct QHidDeviceInfo {
    /** Platform-specific device path */
    QString path;
    /** Device Vendor ID */
    ushort vendorId;
    /** Device Product ID */
    ushort productId;
    /** Serial Number */
    QString serialNumber;
    /** Device Release Number in binary-coded decimal, also known as Device Version Number */
    ushort releaseNumber;
    /** Manufacturer String */
    QString manufacturerString;
    /** Product string */
    QString productString;
#if defined(Q_OS_WIN32) || defined(Q_OS_MAC)
    /** Usage Page for this Device/Interface
            (Windows/Mac only). */
    ushort usagePage;
    /** Usage for this Device/Interface
            (Windows/Mac only).*/
    ushort usage;
#endif
    /** The USB interface which this logical device
            represents. Valid on both Linux implementations
            in all cases, and valid on the Windows implementation
            only if the device contains more than one interface. */
    int interfaceNumber;
};
Q_DECLARE_METATYPE(struct QHidDeviceInfo);


struct udev_hid_attributes {
    char* idVendor;
    char* idProduct;
    char* serial;
    char* manufacturer;
};

// for debug only ...
//struct udev_list_node {
//        struct udev_list_node *next, *prev;
//};
//struct udev_list {
//        struct udev *udev;
//        struct udev_list_node node;
//        struct udev_list_entry **entries;
//        unsigned int entries_cur;
//        unsigned int entries_max;
//        bool unique;
//};
//typedef uint64_t usec_t;
//struct udev_device {
//        struct udev *udev;
//        struct udev_device *parent_device;
//        char *syspath;
//        const char *devpath;
//        char *sysname;
//        const char *sysnum;
//        char *devnode;
//        mode_t devnode_mode;
//        uid_t devnode_uid;
//        gid_t devnode_gid;
//        char *subsystem;
//        char *devtype;
//        char *driver;
//        char *action;
//        char *devpath_old;
//        char *id_filename;
//        char **envp;
//        char *monitor_buf;
//        size_t monitor_buf_len;
//        struct udev_list devlinks_list;
//        struct udev_list properties_list;
//        struct udev_list sysattr_value_list;
//        struct udev_list sysattr_list;
//        struct udev_list tags_list;
//        unsigned long long int seqnum;
//        usec_t usec_initialized;
//        int devlink_priority;
//        int refcount;
//        dev_t devnum;
//        int ifindex;
//        int watch_handle;
//        int maj, min;
//        bool parent_set;
//        bool subsystem_set;
//        bool devtype_set;
//        bool devlinks_uptodate;
//        bool envp_uptodate;
//        bool tags_uptodate;
//        bool driver_set;
//        bool info_loaded;
//        bool db_loaded;
//        bool uevent_loaded;
//        bool is_initialized;
//        bool sysattr_list_read;
//        bool db_persist;
//};

#endif // QHIDDEVICEINFO_H
