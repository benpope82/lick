#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lick.h"

void print_info(sys_info_t *info) {
    printf("family: %s\n", info->family_name);
    printf("version: %s\n", info->version_name);
    printf("architecture: %s\n", info->arch_name);
    printf("bios type: %s\n", info->bios_name);
}

void print_installed(lickdir_t *lick) {
    node_t *l = list_installed(lick);
    for(node_t *n = l; n != NULL; n = n->next) {
        installed_t *i = n->val;
        printf("ID: %s, NAME: %s\n", i->id, i->name);
    }
    free_list_installed(l);
}

void install_iso(char *iso, char *to) {
    sys_info_t *info = get_system_info();
    loader_t *loader = get_loader(info);
    lickdir_t *lick = expand_lick_dir("C:\\lick");
    menu_t *menu = get_menu(loader);

    printf("Before install:\n");
    print_installed(lick);
    install("myID", "myNAME", iso, to, lick, menu);
    print_installed(lick);
    printf("After install:\n");
    getchar();
    uninstall("myID", lick, menu);
    print_installed(lick);
    printf("After uninstall:\n");

    free(menu);
    free(lick);
    free(loader);
    free(info);
}

void loud_test_install(char *iso) {
    sys_info_t *info = get_system_info();
    loader_t *loader = get_loader(info);
    lickdir_t *lick = expand_lick_dir("C:\\lick");

    printf("check: %d\n", check_loader(loader, info));
    printf("install: %d\n", install_loader(loader, info, lick));
    printf("check: %d\n", check_loader(loader, info));
    getchar(); // to modify file
    install_iso("puppy_cli.iso", "C:\\CLI");
    printf("uninstall:%d\n", uninstall_loader(loader, info, lick));
    printf("check: %d\n", check_loader(loader, info));

    free(lick);
    free(loader);
    free(info);
}

void test_install() {
    sys_info_t *info = get_system_info();
    loader_t *loader = get_loader(info);
    lickdir_t *lick = expand_lick_dir("C:\\lick");

    install_loader(loader, info, lick);
    uninstall_loader(loader, info, lick);

    free(lick);
    free(loader);
    free(info);
}

void print_drives() {
    node_t *drv = all_drives();
    for(node_t *n = drv; n != NULL; n = n->next) {
        drive_t *d = n->val;
        printf("- %s\n", d->path);
    }
    free_drive_list(drv);
}

int even(int *i) {
    return (*i % 2 == 0);
}

void test_list() {
    int a=1, b=2, c=3, d=4, e=5;
    node_t *lst =
        new_node(
            &a, new_node(&b, new_node(&c, new_node(&d, new_node(&e, NULL)))));

    assert(list_length(lst) == 5);

    free(lst);
}

int main(int argc, char* argv[]) {
    sys_info_t *info = get_system_info();
    print_info(info);

    test_list();

    if(argc < 2)
        test_install();
    else
        loud_test_install(argv[1]);

    //print_drives();

    return 0;
}
