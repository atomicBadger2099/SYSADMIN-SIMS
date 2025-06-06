#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#define MAX_INPUT 256
#define CLEAR_SCREEN "\033[2J\033[H"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RED "\033[31m"
#define COLOR_CYAN "\033[36m"
#define COLOR_RESET "\033[0m"

// Operating system types
typedef enum {
    OS_DEBIAN,
    OS_UBUNTU,
    OS_SIMULATE_DEBIAN
} os_type_t;

// Global system configuration
typedef struct {
    os_type_t os_type;
    char os_name[50];
    int simulate_mode;
    char prompt_prefix[20];
} system_config_t;

system_config_t sys_config;

// Structure to hold lesson information
typedef struct {
    char title[100];
    char description[300];
    char command[100];
    char explanation[500];
    char simulated_output[1000];
} Lesson;

// Function prototypes
void detect_and_configure_system(void);
void display_welcome(void);
void show_main_menu(void);
void clear_input_buffer(void);
int get_user_choice(int max_options);
void press_enter_to_continue(void);
void show_system_info_lesson(void);
void show_file_system_lesson(void);
void show_process_management_lesson(void);
void show_package_management_lesson(void);
void show_user_management_lesson(void);
void interactive_command_demo(const char* command, const char* description, const char* simulated_output);
void execute_or_simulate_command(const char* command, const char* simulated_output);
char* adapt_command_for_system(const char* original_command);
void show_simulation_notice(void);

int main(void) {
    int choice;
    int running = 1;
    
    detect_and_configure_system();
    display_welcome();
    
    while (running) {
        show_main_menu();
        choice = get_user_choice(6);
        
        switch (choice) {
            case 1:
                show_system_info_lesson();
                break;
            case 2:
                show_file_system_lesson();
                break;
            case 3:
                show_process_management_lesson();
                break;
            case 4:
                show_package_management_lesson();
                break;
            case 5:
                show_user_management_lesson();
                break;
            case 6:
                printf(COLOR_GREEN "\nThanks for learning with us! Keep exploring Linux! 🐧\n" COLOR_RESET);
                running = 0;
                break;
            default:
                printf(COLOR_RED "\nHmm, that's not a valid option. Let's try again!\n" COLOR_RESET);
                press_enter_to_continue();
        }
    }
    
    return 0;
}

void detect_and_configure_system(void) {
    FILE *fp;
    char buffer[256];
    int user_choice;
    
    printf(CLEAR_SCREEN);
    printf(COLOR_CYAN "🔍 System Detection & Configuration\n");
    printf("════════════════════════════════════\n\n" COLOR_RESET);
    
    // Try to detect the system automatically
    fp = fopen("/etc/os-release", "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "ID=", 3) == 0) {
                if (strstr(buffer, "debian")) {
                    printf(COLOR_GREEN "🎯 Detected: Debian system\n" COLOR_RESET);
                    strcpy(sys_config.os_name, "Debian");
                    break;
                } else if (strstr(buffer, "ubuntu")) {
                    printf(COLOR_GREEN "🎯 Detected: Ubuntu system\n" COLOR_RESET);
                    strcpy(sys_config.os_name, "Ubuntu");
                    break;
                }
            }
        }
        fclose(fp);
    }
    
    printf("\nThis tutorial focuses on Debian system administration.\n");
    printf("How would you like to proceed?\n\n");
    
    printf("1. 🐧 I'm on Debian - use real commands\n");
    printf("2. 🟠 I'm on Ubuntu - adapt commands when possible\n");
    printf("3. 🎭 Simulate Debian environment (safe practice mode)\n");
    printf("4. 🤔 I'm not sure - let me choose based on detection\n");
    
    printf(COLOR_BLUE "\nChoose your learning mode (1-4): " COLOR_RESET);
    user_choice = get_user_choice(4);
    
    switch (user_choice) {
        case 1:
            sys_config.os_type = OS_DEBIAN;
            sys_config.simulate_mode = 0;
            strcpy(sys_config.prompt_prefix, "debian");
            printf(COLOR_GREEN "\n✅ Debian mode: Real commands will be executed\n" COLOR_RESET);
            break;
        case 2:
            sys_config.os_type = OS_UBUNTU;
            sys_config.simulate_mode = 0;
            strcpy(sys_config.prompt_prefix, "ubuntu");
            printf(COLOR_GREEN "\n✅ Ubuntu mode: Commands adapted where needed\n" COLOR_RESET);
            break;
        case 3:
            sys_config.os_type = OS_SIMULATE_DEBIAN;
            sys_config.simulate_mode = 1;
            strcpy(sys_config.prompt_prefix, "sim-debian");
            printf(COLOR_GREEN "\n✅ Simulation mode: Safe Debian practice environment\n" COLOR_RESET);
            break;
        case 4:
            if (strstr(sys_config.os_name, "Ubuntu")) {
                sys_config.os_type = OS_UBUNTU;
                sys_config.simulate_mode = 0;
                strcpy(sys_config.prompt_prefix, "ubuntu");
                printf(COLOR_GREEN "\n✅ Auto-selected Ubuntu mode based on detection\n" COLOR_RESET);
            } else {
                sys_config.os_type = OS_DEBIAN;
                sys_config.simulate_mode = 0;
                strcpy(sys_config.prompt_prefix, "debian");
                printf(COLOR_GREEN "\n✅ Auto-selected Debian mode\n" COLOR_RESET);
            }
            break;
    }
    
    if (sys_config.simulate_mode) {
        printf(COLOR_YELLOW "\n🎭 Simulation Mode Active!\n");
        printf("Commands will show realistic Debian outputs without\n");
        printf("actually modifying your system. Perfect for safe learning!\n" COLOR_RESET);
    }
    
    press_enter_to_continue();
}

void display_welcome(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_BLUE "╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║        🐧 Debian SysAdmin Academy! 🐧                     ║\n");
    printf("║                                                            ║\n");
    printf("║    Learn essential Linux system administration skills      ║\n");
    printf("║         through hands-on, interactive lessons             ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n" COLOR_RESET);
    
    printf(COLOR_GREEN "\nHey there, future sysadmin! 👋\n");
    printf("Mode: %s%s%s | System: %s\n", COLOR_CYAN, 
           sys_config.simulate_mode ? "Simulation" : "Live", COLOR_GREEN, sys_config.os_name);
    printf("Ready to dive into Debian system administration?\n");
    printf("We'll explore real commands, understand what they do, and\n");
    printf("build your confidence step by step.\n" COLOR_RESET);
    
    press_enter_to_continue();
}

void show_main_menu(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_YELLOW "🎯 What would you like to explore today?\n");
    printf("Mode: %s | %s\n\n", sys_config.prompt_prefix, 
           sys_config.simulate_mode ? "🎭 Simulation" : "🔥 Live");
    printf(COLOR_RESET);
    
    printf("1. 🖥️  System Information & Monitoring\n");
    printf("2. 📁 File System Navigation & Management\n");
    printf("3. ⚙️  Process Management\n");
    printf("4. 📦 Package Management (APT)\n");
    printf("5. 👥 User & Permission Management\n");
    printf("6. 🚪 Exit\n");
    
    printf(COLOR_BLUE "\nChoose your adventure (1-6): " COLOR_RESET);
}

void show_system_info_lesson(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_YELLOW "🖥️ System Information & Monitoring\n");
    printf("══════════════════════════════════════\n\n" COLOR_RESET);
    
    if (sys_config.simulate_mode) show_simulation_notice();
    
    printf("Let's start by getting to know your system better!\n");
    printf("As a sysadmin, you'll often need to check system status,\n");
    printf("hardware info, and current resource usage.\n\n");
    
    printf(COLOR_GREEN "Would you like to:\n");
    printf("1. See basic system information\n");
    printf("2. Check system resources (CPU, memory, disk)\n");
    printf("3. View system uptime and load\n");
    printf("4. Back to main menu\n" COLOR_RESET);
    
    int choice = get_user_choice(4);
    
    switch (choice) {
        case 1:
            interactive_command_demo("uname -a", 
                "Shows kernel name, version, architecture, and more!",
                "Linux debian-server 6.1.0-13-amd64 #1 SMP PREEMPT_DYNAMIC Debian 6.1.55-1 (2023-09-29) x86_64 GNU/Linux");
            interactive_command_demo("lsb_release -a", 
                "Get detailed Debian version and distribution info",
                "Distributor ID: Debian\nDescription: Debian GNU/Linux 12 (bookworm)\nRelease: 12\nCodename: bookworm");
            interactive_command_demo("hostnamectl", 
                "Modern way to see hostname and system info",
                "Static hostname: debian-server\nIcon name: computer-server\nChassis: server\nMachine ID: a1b2c3d4e5f6789\nBoot ID: x1y2z3a4b5c6d7e8\nOperating System: Debian GNU/Linux 12 (bookworm)\nKernel: Linux 6.1.0-13-amd64\nArchitecture: x86-64");
            break;
        case 2:
            interactive_command_demo("free -h", 
                "Memory usage in human-readable format",
                "              total        used        free      shared  buff/cache   available\nMem:           15Gi       2.1Gi        10Gi       256Mi       3.2Gi        12Gi\nSwap:         2.0Gi          0B       2.0Gi");
            interactive_command_demo("df -h", 
                "Disk space usage for all mounted filesystems",
                "Filesystem      Size  Used Avail Use% Mounted on\n/dev/sda1        20G  8.5G   10G  46% /\n/dev/sda2       100G   45G   50G  48% /home\ntmpfs           7.8G     0  7.8G   0% /dev/shm");
            interactive_command_demo("lscpu", 
                "Detailed CPU architecture information",
                "Architecture:        x86_64\nCPU op-mode(s):      32-bit, 64-bit\nByte Order:          Little Endian\nCPU(s):              4\nCore(s) per socket:  2\nSocket(s):           2\nModel name:          Intel(R) Core(TM) i7-8565U CPU @ 1.80GHz");
            break;
        case 3:
            interactive_command_demo("uptime", 
                "System uptime and load averages",
                " 14:32:15 up 7 days, 12:45,  3 users,  load average: 0.15, 0.23, 0.18");
            interactive_command_demo("w", 
                "Who's logged in and what they're doing",
                " 14:32:16 up 7 days, 12:45,  3 users,  load average: 0.15, 0.23, 0.18\nUSER     TTY      FROM             LOGIN@   IDLE   JCPU   PCPU WHAT\nroot     pts/0    192.168.1.100    13:45    2:00   0.01s  0.01s -bash\nadmin    pts/1    192.168.1.101    14:30    0.00s  0.05s  0.01s w");
            interactive_command_demo("top -n 1 | head -10", 
                "Quick snapshot of running processes",
                "top - 14:32:17 up 7 days, 12:45,  3 users,  load average: 0.15, 0.23, 0.18\nTasks: 127 total,   1 running, 126 sleeping,   0 stopped,   0 zombie\n%Cpu(s):  2.3 us,  1.2 sy,  0.0 ni, 96.2 id,  0.3 wa,  0.0 hi,  0.0 si,  0.0 st\nMiB Mem :  15925.7 total,  10234.5 free,   2156.8 used,   3534.4 buff/cache\nMiB Swap:   2048.0 total,   2048.0 free,      0.0 used.  12987.2 avail Mem\n\n  PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND\n    1 root      20   0  167304  13524   8456 S   0.0   0.1   0:03.21 systemd\n    2 root      20   0       0      0      0 S   0.0   0.0   0:00.01 kthreadd");
            break;
        case 4:
            return;
    }
    
    printf(COLOR_GREEN "\n💡 Pro tip: Combine commands with pipes!\n");
    printf("Try: ps aux | grep nginx\n");
    printf("This finds all processes related to nginx.\n" COLOR_RESET);
    
    press_enter_to_continue();
}

void show_file_system_lesson(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_YELLOW "📁 File System Navigation & Management\n");
    printf("═══════════════════════════════════════════\n\n" COLOR_RESET);
    
    if (sys_config.simulate_mode) show_simulation_notice();
    
    printf("File system mastery is crucial for any sysadmin!\n");
    printf("Let's explore navigation, file operations, and permissions.\n\n");
    
    printf(COLOR_GREEN "What interests you most?\n");
    printf("1. Navigation basics (pwd, ls, cd)\n");
    printf("2. File operations (cp, mv, rm, mkdir)\n");
    printf("3. Finding files and content (find, grep, locate)\n");
    printf("4. File permissions and ownership\n");
    printf("5. Back to main menu\n" COLOR_RESET);
    
    int choice = get_user_choice(5);
    
    switch (choice) {
        case 1:
            printf("\n🧭 Let's navigate like a pro!\n\n");
            interactive_command_demo("pwd", 
                "Print Working Directory - where am I?",
                "/home/admin");
            interactive_command_demo("ls -la", 
                "List all files with detailed info (including hidden ones)",
                "total 32\ndrwxr-xr-x 4 admin admin 4096 Oct 15 14:30 .\ndrwxr-xr-x 3 root  root  4096 Oct 10 09:15 ..\n-rw------- 1 admin admin  220 Oct 10 09:15 .bash_logout\n-rw------- 1 admin admin 3526 Oct 10 09:15 .bashrc\ndrwx------ 2 admin admin 4096 Oct 15 14:25 .ssh\n-rw-r--r-- 1 admin admin  807 Oct 10 09:15 .profile\ndrwxr-xr-x 2 admin admin 4096 Oct 15 12:30 Documents");
            interactive_command_demo("ls -lh /etc | head -10", 
                "Look inside /etc with human-readable file sizes",
                "total 1.2M\ndrwxr-xr-x   3 root root    4.0K Oct 10 09:20 alternatives\n-rw-r--r--   1 root root    2.9K Oct 10 09:15 bash.bashrc\n-rw-r--r--   1 root root     367 Jan 27  2023 bindresvport.blacklist\ndrwxr-xr-x   2 root root    4.0K Oct 15 10:30 cron.d\ndrwxr-xr-x   2 root root    4.0K Oct 15 10:30 cron.daily\n-rw-r--r--   1 root root    2.9K Jan 26  2023 debconf.conf\ndrwxr-xr-x   2 root root    4.0K Oct 10 09:20 default\n-rw-r--r--   1 root root     604 Jul  2  2023 deluser.conf");
            break;
        case 2:
            printf("\n🔧 File manipulation essentials!\n\n");
            printf(COLOR_BLUE "Note: In simulation mode, these are examples - be careful with real file operations!\n" COLOR_RESET);
            interactive_command_demo("mkdir -p ~/test/nested/dir", 
                "Create nested directories in one command",
                sys_config.simulate_mode ? "Created directories: /home/admin/test/nested/dir" : "");
            interactive_command_demo("touch ~/test/example.txt", 
                "Create an empty file or update timestamp",
                sys_config.simulate_mode ? "Created file: /home/admin/test/example.txt" : "");
            interactive_command_demo("ls -la ~/test/", 
                "Check our created directory",
                "total 12\ndrwxr-xr-x 3 admin admin 4096 Oct 15 14:35 .\ndrwxr-xr-x 5 admin admin 4096 Oct 15 14:35 ..\n-rw-r--r-- 1 admin admin    0 Oct 15 14:35 example.txt\ndrwxr-xr-x 3 admin admin 4096 Oct 15 14:35 nested");
            break;
        case 3:
            printf("\n🔍 Finding files like a detective!\n\n");
            interactive_command_demo("find /etc -name '*.conf' | head -5", 
                "Find first 5 .conf files in /etc directory",
                "/etc/adduser.conf\n/etc/debconf.conf\n/etc/deluser.conf\n/etc/fuse.conf\n/etc/host.conf");
            interactive_command_demo("locate sshd_config", 
                "Quick search using the locate database",
                "/etc/ssh/sshd_config");
            interactive_command_demo("grep -r 'error' /var/log/ | head -3", 
                "Search for 'error' in log files (first 3 results)",
                "/var/log/syslog:Oct 15 10:30:15 debian kernel: [12345.678] USB disconnect error\n/var/log/auth.log:Oct 15 12:15:30 debian sshd[1234]: Authentication error for user test\n/var/log/daemon.log:Oct 15 13:45:22 debian systemd[1]: Service error: failed to start");
            break;
        case 4:
            printf("\n🔐 Permissions and ownership!\n\n");
            interactive_command_demo("ls -l /etc/passwd", 
                "Check permissions on an important system file",
                "-rw-r--r-- 1 root root 2847 Oct 10 09:20 /etc/passwd");
            printf(COLOR_BLUE "\nPermission format: rwxrwxrwx (user-group-other)\n");
            printf("r=read(4), w=write(2), x=execute(1)\n" COLOR_RESET);
            interactive_command_demo("stat /etc/passwd", 
                "Detailed file information including permissions",
                "  File: /etc/passwd\n  Size: 2847      \tBlocks: 8          IO Block: 4096   regular file\nDevice: 801h/2049d\tInode: 131074      Links: 1\nAccess: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)\nAccess: 2023-10-15 10:30:15.123456789 +0000\nModify: 2023-10-10 09:20:45.987654321 +0000\nChange: 2023-10-10 09:20:45.987654321 +0000");
            break;
        case 5:
            return;
    }
    
    press_enter_to_continue();
}

void show_process_management_lesson(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_YELLOW "⚙️ Process Management\n");
    printf("══════════════════════\n\n" COLOR_RESET);
    
    if (sys_config.simulate_mode) show_simulation_notice();
    
    printf("Understanding processes is key to system administration!\n");
    printf("Let's learn to monitor, control, and troubleshoot processes.\n\n");
    
    printf(COLOR_GREEN "Choose your focus:\n");
    printf("1. Viewing processes (ps, top, htop)\n");
    printf("2. Process control (kill, killall, jobs)\n");
    printf("3. System services (systemctl)\n");
    printf("4. Back to main menu\n" COLOR_RESET);
    
    int choice = get_user_choice(4);
    
    switch (choice) {
        case 1:
            printf("\n👀 Let's see what's running!\n\n");
            interactive_command_demo("ps aux | head -10", 
                "Show first 10 running processes with detailed info",
                "USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND\nroot         1  0.0  0.1 167304 13524 ?        Ss   Oct08   0:03 /sbin/init\nroot         2  0.0  0.0      0     0 ?        S    Oct08   0:00 [kthreadd]\nroot         3  0.0  0.0      0     0 ?        I<   Oct08   0:00 [rcu_gp]\nroot         4  0.0  0.0      0     0 ?        I<   Oct08   0:00 [rcu_par_gp]\nroot         6  0.0  0.0      0     0 ?        I<   Oct08   0:00 [kworker/0:0H-events_highpri]\nroot         9  0.0  0.0      0     0 ?        I<   Oct08   0:00 [mm_percpu_wq]\nroot        10  0.0  0.0      0     0 ?        S    Oct08   0:00 [rcu_tasks_rude_]\nsystemd+   123  0.0  0.1  24756 12234 ?        Ss   Oct08   0:15 /lib/systemd/systemd-resolved\nroot       456  0.0  0.2  72456 23456 ?        Ss   Oct08   0:08 /usr/sbin/sshd -D");
            interactive_command_demo("pgrep -l ssh", 
                "Find processes by name (ssh in this case)",
                "456 sshd\n789 ssh-agent");
            break;
        case 2:
            printf("\n🎮 Process control commands!\n\n");
            printf(COLOR_RED "⚠️  Be very careful with kill commands in live mode!\n" COLOR_RESET);
            interactive_command_demo("jobs", 
                "Show active jobs in current shell",
                sys_config.simulate_mode ? "[1]+  Stopped                 vim /etc/hosts\n[2]-  Running                 tail -f /var/log/syslog &" : "");
            printf(COLOR_BLUE "\nCommon signals:\n");
            printf("SIGTERM (15) - Polite shutdown request\n");
            printf("SIGKILL (9) - Force termination (use sparingly!)\n");
            printf("SIGHUP (1) - Hang up (often reloads config)\n" COLOR_RESET);
            if (sys_config.simulate_mode) {
                printf(COLOR_YELLOW "\nIn simulation: 'kill -15 1234' would send SIGTERM to PID 1234\n" COLOR_RESET);
            }
            break;
        case 3:
            printf("\n🔧 Managing system services!\n\n");
            interactive_command_demo("systemctl status ssh", 
                "Check SSH service status",
                "● ssh.service - OpenBSD Secure Shell server\n   Loaded: loaded (/lib/systemd/system/ssh.service; enabled; vendor preset: enabled)\n   Active: active (running) since Tue 2023-10-10 09:20:15 UTC; 5 days ago\n     Docs: man:sshd(8)\n           man:sshd_config(5)\n  Process: 456 ExecStartPre=/usr/sbin/sshd -t (code=exited, status=0/SUCCESS)\n Main PID: 456 (sshd)\n    Tasks: 1 (limit: 4915)\n   Memory: 5.2M\n      CPU: 1.234s\n   CGroup: /system.slice/ssh.service\n           └─456 /usr/sbin/sshd -D");
            interactive_command_demo("systemctl list-units --type=service --state=running | head -10", 
                "List first 10 running services",
                "UNIT                               LOAD   ACTIVE SUB     DESCRIPTION\ncron.service                       loaded active running Regular background program processing daemon\ndbus.service                       loaded active running D-Bus System Message Bus\nnetworkd-dispatcher.service        loaded active running Dispatcher daemon for systemd-networkd\nnetworking.service                 loaded active exited  Raise network interfaces\nrsyslog.service                    loaded active running System Logging Service\nssh.service                        loaded active running OpenBSD Secure Shell server\nsystemd-journald.service           loaded active running Journal Service\nsystemd-logind.service             loaded active running User Login Management\nsystemd-networkd.service           loaded active running Network Configuration");
            printf(COLOR_BLUE "\nCommon systemctl commands:\n");
            printf("start, stop, restart, enable, disable, status\n" COLOR_RESET);
            break;
        case 4:
            return;
    }
    
    press_enter_to_continue();
}

void show_package_management_lesson(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_YELLOW "📦 Package Management with APT\n");
    printf("═══════════════════════════════\n\n" COLOR_RESET);
    
    if (sys_config.simulate_mode) show_simulation_notice();
    
    printf("APT (Advanced Package Tool) is Debian's package manager.\n");
    printf("It's your gateway to installing, updating, and managing software!\n");
    
    if (sys_config.os_type == OS_UBUNTU) {
        printf(COLOR_CYAN "Note: You're on Ubuntu - APT works the same way! 🎉\n" COLOR_RESET);
    }
    printf("\n");
    
    printf(COLOR_GREEN "What would you like to learn?\n");
    printf("1. Package searching and information\n");
    printf("2. Installing and removing packages\n");
    printf("3. System updates and upgrades\n");
    printf("4. Package dependencies and troubleshooting\n");
    printf("5. Back to main menu\n" COLOR_RESET);
    
    int choice = get_user_choice(5);
    
    switch (choice) {
        case 1:
            printf("\n🔍 Exploring available packages!\n\n");
            interactive_command_demo("apt search htop", 
                "Search for packages containing 'htop'",
                "Sorting... Done\nFull Text Search... Done\nhtop/stable 3.2.1-1 amd64\n  interactive processes viewer\n\nhtop-vim/stable 1.0.2-1 all\n  Vi-style key bindings for htop");
            interactive_command_demo("apt show htop", 
                "Detailed information about htop package",
                "Package: htop\nVersion: 3.2.1-1\nPriority: optional\nSection: utils\nMaintainer: Daniel Lange <DLange@debian.org>\nInstalled-Size: 234 kB\nDepends: libc6 (>= 2.15), libncurses6 (>= 6), libtinfo6 (>= 6)\nHomepage: https://htop.dev/\nDescription: interactive processes viewer\n htop is a ncurses-based process viewer similar to top, but it\n allows one to scroll the list vertically and horizontally to see\n all processes and their full command lines.");
            interactive_command_demo("dpkg -l | grep vim", 
                "Check if vim packages are installed",
                "ii  vim-common    2:9.0.1378-2    all          Vi IMproved - Common files\nii  vim-tiny      2:9.0.1378-2    amd64        Vi IMproved - enhanced vi editor - compact version");
            break;
        case 2:
            printf("\n📥 Installing and removing software!\n\n");
            if (!sys_config.simulate_mode) {
                printf(COLOR_RED "⚠️  These commands require root privileges (sudo)!\n" COLOR_RESET);
            }
            interactive_command_demo("sudo apt install htop", 
                "Install htop system monitor",
                "Reading package lists... Done\nBuilding dependency tree... Done\nReading state information... Done\nThe following NEW packages will be installed:\n  htop\n0 upgraded, 1 newly installed, 0 to remove and 0 not upgraded.\nNeed to get 123 kB of archives.\nAfter this operation, 234 kB of additional disk space will be used.\nGet:1 http://deb.debian.org/debian bookworm/main amd64 htop amd64 3.2.1-1 [123 kB]\nFetched 123 kB in 1s (123 kB/s)\nSelecting previously unselected package htop.\n(Reading database ... 95432 files and directories currently installed.)\nPreparing to unpack .../htop_3.2.1-1_amd64.deb ...\nUnpacking htop (3.2.1-1) ...\nSetting up htop (3.2.1-1) ...\nProcessing triggers for man-db (2.11.2-2) ...");
            interactive_command_demo("sudo apt remove htop", 
                "Remove htop (keeps config files)",
                "Reading package lists... Done\nBuilding dependency tree... Done\nReading state information... Done\nThe following packages will be REMOVED:\n  htop\n0 upgraded, 0 newly installed, 1 to remove and 0 not upgraded.\nAfter this operation, 234 kB disk space will be freed.\nDo you want to continue? [Y/n] Y\n(Reading database ... 95456 files and directories currently installed.)\nRemoving htop (3.2.1-1) ...\nProcessing triggers for man-db (2.11.2-2) ...");
            interactive_command_demo("sudo apt purge htop", 
                "Remove htop and its configuration files",
                "Reading package lists... Done\nBuilding dependency tree... Done\nReading state information... Done\nPackage 'htop' is not installed, so not removed\n0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.");
            break;
        case 3:
            printf("\n🔄 Keeping your system updated!\n\n");
            interactive_command_demo("sudo apt update", 
                "Update package list from repositories",
                "Hit:1 http://security.debian.org/debian-security bookworm-security InRelease\nHit:2 http://deb.debian.org/debian bookworm InRelease\nHit:3 http://deb.debian.org/debian bookworm-updates InRelease\nReading package lists... Done\nBuilding dependency tree... Done\nReading state information... Done\n15 packages can be upgraded. Run 'apt list --upgradable' to see them.");
            interactive_command_demo("apt list --upgradable", 
                "See what packages can be upgraded",
                "Listing... Done\nbase-files/stable 12.4+deb12u2 amd64 [upgradable from: 12.4+deb12u1]\nlibc6/stable 2.36-9+deb12u3 amd64 [upgradable from: 2.36-9+deb12u2]\nlibc6-dev/stable 2.36-9+deb12u3 amd64 [upgradable from: 2.36-9+deb12u2]\nlinux-image-amd64/stable 6.1.55-1 amd64 [upgradable from: 6.1.52-1]\nvim-common/stable 2:9.0.1378-2 all [upgradable from: 2:9.0.1378-1]");
            interactive_command_demo("sudo apt upgrade", 
                "Upgrade installed packages to newer versions",
                "Reading package lists... Done\nBuilding dependency tree... Done\nReading state information... Done\nCalculating upgrade... Done\nThe following packages will be upgraded:\n  base-files libc6 libc6-dev linux-image-amd64 vim-common\n5 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.\nNeed to get 23.4 MB of archives.\nAfter this operation, 156 kB of additional disk space will be used.\nDo you want to continue? [Y/n] Y");
            printf(COLOR_GREEN "\n💡 Best practice: Always 'apt update' before 'apt upgrade'!\n" COLOR_RESET);
            break;
        case 4:
            printf("\n🔧 Dependency management!\n\n");
            interactive_command_demo("apt depends firefox-esr", 
                "Show what firefox-esr depends on",
                "firefox-esr\n  Depends: libasound2\n  Depends: libatk-1.0-0\n  Depends: libc6\n  Depends: libcairo-gobject2\n  Depends: libcairo2\n  Depends: libdbus-1-3\n  Depends: libfontconfig1\n  Depends: libfreetype6\n  Depends: libgcc-s1\n  Depends: libgdk-pixbuf-2.0-0\n  Depends: libglib2.0-0\n  Depends: libgtk-3-0\n  Depends: libpango-1.0-0\n  Depends: libstdc++6\n  Depends: libx11-6\n  Depends: libxcomposite1\n  Depends: libxdamage1\n  Depends: libxext6\n  Depends: libxfixes3\n  Depends: libxrandr2\n  Depends: libxrender1\n  Depends: libxtst6");
            interactive_command_demo("apt rdepends libc6 | head -10", 
                "Show first 10 packages that depend on libc6",
                "libc6\nReverse Depends:\n  zutils\n  zstd\n  zsh-common\n  zsh\n  zip\n  zile\n  zenity-common\n  zenity");
            interactive_command_demo("sudo apt autoremove", 
                "Remove packages that are no longer needed",
                "Reading package lists... Done\nBuilding dependency tree... Done\nReading state information... Done\n0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.");
            break;
        case 5:
            return;
    }
    
    press_enter_to_continue();
}

void show_user_management_lesson(void) {
    printf(CLEAR_SCREEN);
    printf(COLOR_YELLOW "👥 User & Permission Management\n");
    printf("════════════════════════════════\n\n" COLOR_RESET);
    
    if (sys_config.simulate_mode) show_simulation_notice();
    
    printf("User management is a core sysadmin responsibility!\n");
    printf("Let's explore users, groups, and permissions.\n\n");
    
    printf(COLOR_GREEN "Pick your area of interest:\n");
    printf("1. Viewing users and groups\n");
    printf("2. User account management\n");
    printf("3. File permissions deep dive\n");
    printf("4. sudo and privilege escalation\n");
    printf("5. Back to main menu\n" COLOR_RESET);
    
    int choice = get_user_choice(5);
    
    switch (choice) {
        case 1:
            printf("\n👁️ Who's on this system?\n\n");
            interactive_command_demo("whoami", 
                "Current username",
                "admin");
            interactive_command_demo("id", 
                "Your user ID and group memberships",
                "uid=1000(admin) gid=1000(admin) groups=1000(admin),24(cdrom),25(floppy),27(sudo),29(audio),30(dip),44(video),46(plugdev),108(netdev),114(bluetooth),119(lpadmin),134(scanner)");
            interactive_command_demo("getent passwd | tail -5", 
                "Last 5 entries in the user database",
                "systemd-coredump:x:999:999:systemd Core Dumper:/:/usr/sbin/nologin\nsystemd-network:x:998:998:systemd Network Management:/:/usr/sbin/nologin\nsystemd-resolve:x:997:997:systemd Resolver:/:/usr/sbin/nologin\nsystemd-timesync:x:996:996:systemd Time Synchronization:/:/usr/sbin/nologin\nadmin:x:1000:1000:System Administrator,,,:/home/admin:/bin/bash");
            interactive_command_demo("getent group sudo", 
                "See who's in the sudo group",
                "sudo:x:27:admin");
            break;
        case 2:
            printf("\n👤 User account commands!\n\n");
            if (!sys_config.simulate_mode) {
                printf(COLOR_RED "⚠️  These require root privileges and affect system security!\n" COLOR_RESET);
            }
            interactive_command_demo("sudo adduser newuser", 
                "Interactive way to add a new user",
                "Adding user `newuser' ...\nAdding new group `newuser' (1001) ...\nAdding new user `newuser' (1001) with group `newuser' ...\nCreating home directory `/home/newuser' ...\nCopying files from `/etc/skel' ...\nNew password: \nRetype new password: \npasswd: password updated successfully\nChanging the user information for newuser\nEnter the new value, or press ENTER for the default\n\tFull Name []: New User\n\tRoom Number []: \n\tWork Phone []: \n\tHome Phone []: \n\tOther []: \nIs the information correct? [Y/n] Y");
            interactive_command_demo("sudo usermod -aG sudo newuser", 
                "Add user to sudo group",
                sys_config.simulate_mode ? "User 'newuser' added to group 'sudo'" : "");
            interactive_command_demo("sudo passwd newuser", 
                "Change a user's password",
                "New password: \nRetype new password: \npasswd: password updated successfully");
            break;
        case 3:
            printf("\n🔐 Permission mastery!\n\n");
            interactive_command_demo("ls -la ~/.bashrc", 
                "Check permissions on your shell config",
                "-rw-r--r-- 1 admin admin 3526 Oct 10 09:15 /home/admin/.bashrc");
            printf(COLOR_BLUE "\nPermission notation:\n");
            printf("4 = read, 2 = write, 1 = execute\n");
            printf("755 = rwxr-xr-x (owner: rwx, group: r-x, other: r-x)\n");
            printf("644 = rw-r--r-- (owner: rw-, group: r--, other: r--)\n" COLOR_RESET);
            interactive_command_demo("umask", 
                "Default permission mask for new files",
                "0022");
            printf(COLOR_GREEN "\n💡 umask 0022 means new files get 644 permissions,\n");
            printf("new directories get 755 permissions.\n" COLOR_RESET);
            break;
        case 4:
            printf("\n🔑 Understanding sudo!\n\n");
            interactive_command_demo("sudo -l", 
                "What can you run with sudo?",
                "Matching Defaults entries for admin on debian-server:\n    env_reset, mail_badpass, secure_path=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin, use_pty\n\nUser admin may run the following commands on debian-server:\n    (ALL : ALL) ALL");
            interactive_command_demo("ls -la /etc/sudoers", 
                "Check sudoers file permissions",
                "-r--r----- 1 root root 1042 Oct 10 09:20 /etc/sudoers");
            printf(COLOR_GREEN "\n💡 Sudo tips:\n");
            printf("- Use 'sudo -i' for a root shell (be careful!)\n");
            printf("- Use 'sudo visudo' to edit sudoers file safely\n");
            printf("- Prefer individual sudo commands for better security!\n" COLOR_RESET);
            break;
        case 5:
            return;
    }
    
    press_enter_to_continue();
}

void interactive_command_demo(const char* command, const char* description, const char* simulated_output) {
    printf(COLOR_BLUE "\n📋 Command: " COLOR_YELLOW "%s\n" COLOR_RESET, command);
    printf(COLOR_GREEN "What it does: %s\n" COLOR_RESET, description);
    
    printf("\nWould you like to:\n");
    printf("1. Run this command now\n");
    printf("2. Just see the explanation\n");
    printf("3. Skip to next\n");
    
    int choice = get_user_choice(3);
    
    if (choice == 1) {
        execute_or_simulate_command(command, simulated_output);
    } else if (choice == 2) {
        printf(COLOR_BLUE "\n📖 More details:\n%s\n" COLOR_RESET, description);
    }
    
    printf("\n");
}

void execute_or_simulate_command(const char* command, const char* simulated_output) {
    printf(COLOR_YELLOW "\n🚀 %s: %s\n", 
           sys_config.simulate_mode ? "Simulating" : "Running", command);
    printf("───────────────────────────────────────\n" COLOR_RESET);
    
    if (sys_config.simulate_mode) {
        if (strlen(simulated_output) > 0) {
            printf("%s\n", simulated_output);
        } else {
            printf(COLOR_CYAN "[Simulated - command would execute safely]\n" COLOR_RESET);
        }
        printf("───────────────────────────────────────\n");
        printf(COLOR_GREEN "✅ Simulation completed successfully!\n" COLOR_RESET);
    } else {
        // Adapt command for current system if needed
        char* adapted_command = adapt_command_for_system(command);
        
        // Execute the command
        int result = system(adapted_command);
        
        printf("───────────────────────────────────────\n");
        if (result == 0) {
            printf(COLOR_GREEN "✅ Command completed successfully!\n" COLOR_RESET);
        } else {
            printf(COLOR_RED "⚠️  Command had issues (exit code: %d)\n" COLOR_RESET, result);
            printf("This might be normal depending on your system setup.\n");
        }
        
        if (adapted_command != command) {
            free(adapted_command);
        }
    }
}

char* adapt_command_for_system(const char* original_command) {
    // For now, most commands work the same on Ubuntu and Debian
    // This function can be extended to handle system-specific adaptations
    
    if (sys_config.os_type == OS_UBUNTU) {
        // Ubuntu-specific adaptations could go here
        // For example, different package names, paths, etc.
        
        // Most APT commands work identically
        // File system commands are the same
        // systemctl commands are the same
    }
    
    // Return the original command (no adaptation needed for most cases)
    return (char*)original_command;
}

void show_simulation_notice(void) {
    printf(COLOR_CYAN "🎭 SIMULATION MODE: Commands will show example outputs without affecting your system\n\n" COLOR_RESET);
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_user_choice(int max_options) {
    int choice;
    char input[MAX_INPUT];
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= max_options) {
                return choice;
            }
        }
        printf(COLOR_RED "Please enter a number between 1 and %d: " COLOR_RESET, max_options);
    }
}

void press_enter_to_continue(void) {
    printf(COLOR_BLUE "\nPress Enter to continue..." COLOR_RESET);
    getchar();
}
