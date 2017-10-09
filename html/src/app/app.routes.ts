import { Routes } from '@angular/router';
import { NoContentComponent } from './no-content';
import {OffComponent} from "./off/off.component";

export const ROUTES: Routes = [
    { path: 'off', component: OffComponent},
    { path: 'manual', loadChildren: './manual/manual.module#ManualModule'},
    { path: 'race', loadChildren: './observer-tabs/observer-tabs.module#ObserverTabsModule'},
    { path: 'error', loadChildren: './error/errors.module#ErrorModule' },
    { path: '**', component: NoContentComponent },
];
