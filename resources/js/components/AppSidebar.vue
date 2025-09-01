<script setup lang="ts">
import NavFooter from '@/components/NavFooter.vue';
import NavUser from '@/components/NavUser.vue';
import { Sidebar, SidebarContent, SidebarFooter, SidebarHeader, SidebarMenu, SidebarMenuButton, SidebarMenuItem, SidebarGroup, SidebarGroupLabel, SidebarMenuSub, SidebarMenuSubItem, SidebarMenuSubButton } from '@/components/ui/sidebar';
import { type NavItem } from '@/types';
interface NavGroupItem extends NavItem {
    children?: NavItem[];
}
import { Link, usePage } from '@inertiajs/vue3';
import { BookOpen, Folder, LayoutGrid, Users, History, CreditCard, ScanLine, ChevronRight } from 'lucide-vue-next';
import { ref, onMounted, watch } from 'vue';
import AppLogo from './AppLogo.vue';

// State untuk toggle collapse/expand
const expandedMenus = ref<Set<string>>(new Set())
const page = usePage()

const toggleMenu = (menuTitle: string) => {
    if (expandedMenus.value.has(menuTitle)) {
        expandedMenus.value.delete(menuTitle)
    } else {
        expandedMenus.value.add(menuTitle)
    }
    // Simpan ke localStorage
    localStorage.setItem('expandedMenus', JSON.stringify(Array.from(expandedMenus.value)))
}

// Load state dari localStorage
onMounted(() => {
    const saved = localStorage.getItem('expandedMenus')
    if (saved) {
        expandedMenus.value = new Set(JSON.parse(saved))
    }
    checkActiveMenu()
})

// Auto-expand menu jika ada child yang aktif
const checkActiveMenu = () => {
    const currentUrl = page.url
    navGroups.forEach(group => {
        group.items.forEach(item => {
            if (item.children) {
                const hasActiveChild = item.children.some(child => child.href === currentUrl)
                if (hasActiveChild) {
                    expandedMenus.value.add(item.title)
                }
            }
        })
    })
}

// Watch perubahan route untuk auto-expand
watch(() => page.url, () => {
    checkActiveMenu()
})

const navGroups: { label: string; items: NavGroupItem[] }[] = [
    {
        label: 'General',
        items: [
            {
                title: 'Dashboard',
                href: '/dashboard',
                icon: LayoutGrid,
            },
        ],
    },
    {
        label: 'Management',
        items: [
            {
                title: 'Data Aslab',
                href: '/aslabs',
                icon: Users,
            },
        ],
    },
    {
        label: 'Attendance',
        items: [
            {
                title: 'Absensi Piket',
                href: '#',
                icon: ScanLine,
                children: [
                    {
                        title: 'Scan Absensi',
                        href: '/attendance-scan',
                        icon: ScanLine,
                    },
                    {
                        title: 'Riwayat Absensi',
                        href: '/attendance-history',
                        icon: History,
                    },
                    {
                        title: 'Registrasi RFID',
                        href: '/rfid-registration',
                        icon: CreditCard,
                    },
                ],
            },
        ],
    },
];

const footerNavItems: NavItem[] = [
    {
        title: 'Github Repo',
        href: 'https://github.com/laravel/vue-starter-kit',
        icon: Folder,
    },
    {
        title: 'Documentation',
        href: 'https://laravel.com/docs/starter-kits#vue',
        icon: BookOpen,
    },
];
</script>

<template>
    <Sidebar collapsible="icon" variant="inset">
        <SidebarHeader>
            <SidebarMenu>
                <SidebarMenuItem>
                    <SidebarMenuButton size="lg" as-child>
                        <Link :href="route('dashboard')">
                            <AppLogo />
                        </Link>
                    </SidebarMenuButton>
                </SidebarMenuItem>
            </SidebarMenu>
        </SidebarHeader>

        <SidebarContent>
            <div class="px-2 py-0">
                <template v-for="group in navGroups" :key="group.label">
                    <SidebarGroup>
                        <SidebarGroupLabel>{{ group.label }}</SidebarGroupLabel>
                        <SidebarMenu>
                            <SidebarMenuItem v-for="item in group.items" :key="item.title">
                                <template v-if="item.children">
                                    <SidebarMenuButton
                                        @click="toggleMenu(item.title)"
                                        :tooltip="item.title"
                                    >
                                        <component :is="item.icon" />
                                        <span>{{ item.title }}</span>
                                        <ChevronRight
                                            class="ml-auto h-4 w-4 transition-transform duration-200"
                                            :class="{ 'rotate-90': expandedMenus.has(item.title) }"
                                        />
                                    </SidebarMenuButton>
                                    <SidebarMenuSub v-show="expandedMenus.has(item.title)">
                                        <SidebarMenuSubItem v-for="child in item.children" :key="child.title">
                                            <SidebarMenuSubButton as-child :is-active="child.href === page.url">
                                                <Link :href="child.href">
                                                    <component :is="child.icon" />
                                                    <span>{{ child.title }}</span>
                                                </Link>
                                            </SidebarMenuSubButton>
                                        </SidebarMenuSubItem>
                                    </SidebarMenuSub>
                                </template>
                                <template v-else>
                                    <SidebarMenuButton as-child :tooltip="item.title">
                                        <Link :href="item.href">
                                            <component :is="item.icon" />
                                            <span>{{ item.title }}</span>
                                        </Link>
                                    </SidebarMenuButton>
                                </template>
                            </SidebarMenuItem>
                        </SidebarMenu>
                    </SidebarGroup>
                </template>
            </div>
        </SidebarContent>

        <SidebarFooter>
            <NavFooter :items="footerNavItems" />
            <NavUser />
        </SidebarFooter>
    </Sidebar>
    <slot />
</template>
