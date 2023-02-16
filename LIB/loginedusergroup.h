#ifndef LoginedUserGroup_H
#define LoginedUserGroup_H

enum LoginedUserGroup {
    NoLoginedUser         = 0x00,
    Login_found_as_powerUser  = 0x01, /*see: SMO::AccessState_and_userPassStatus_forecast() */
    Login_found_as_owner      = 0x02,
    Login_found_as_tuner      = 0x03,
//...
    Logined_as_user       = 0x04,
    Logined_as_powerUser  = 0x05,
    Logined_as_owner      = 0x06,
    Logined_as_tuner      = 0x07
};

#define smoLoginAccepted_bm   0x04
#define UserGroup_mask        0x03
#define LoginedUserGroup_mask 0x07
#define SmoKey2LoginedUserGroup(smo_key) ((smo_key) & LoginedUserGroup_mask)
#define UserGroup_masked(lug) ((lug) & UserGroup_mask)


#endif // LoginedUserGroup_H
