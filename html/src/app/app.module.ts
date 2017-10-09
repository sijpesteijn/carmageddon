import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import {
    NgModule,
    ApplicationRef, APP_INITIALIZER
} from '@angular/core';
import {
    removeNgStyles,
    createNewHosts,
    createInputTransfer
} from '@angularclass/hmr';
import {
    RouterModule,
    PreloadAllModules
} from '@angular/router';

/*
 * Platform and Environment providers/directives/pipes
 */
import { ENV_PROVIDERS } from './environment';
import { ROUTES } from './app.routes';
// App is our top level component
import { AppComponent } from './app.component';
// import { APP_RESOLVER_PROVIDERS } from './app.resolver';
import { AppState, InternalStateType } from './app.service';
import { NoContentComponent } from './no-content';

import '../styles/styles.scss';
import '../styles/headings.css';
import { ObserverModule } from './observers/observer.module';
import { LifeLineComponent } from './lifeline/lifeline.component';
import { Config } from './app.config';
import { ObserverTabsModule } from './observer-tabs/observer-tabs.module';
import { StreamModule } from './webcam/stream.module';
import { LifeLineModule } from './lifeline/lifeline.module';
import { WebcamModule } from './webcam/webcam.module';
import {CarService} from "./car.service";
import {OffComponent} from "./off/off.component";
import {HeaderComponent} from "./header/header.component";
import {CarStatusComponent} from "./car-status/car-status.component";
import {NgxVirtualJoystickModule} from "ngx-virtual-joystick";

// Application wide providers
const APP_PROVIDERS = [
    // ...APP_RESOLVER_PROVIDERS,
    AppState
];

type StoreType = {
    state: InternalStateType,
    restoreInputValues: () => void,
    disposeOldHosts: () => void
};

let PROVIDERS = [
];

if ('local' === process.env.CONTEXT) {
    PROVIDERS = [
        { provide: 'settings.path', useValue: './assets/config/application.local.json' },
        { provide: APP_INITIALIZER, useFactory: (config: Config) => () => config.load('/assets/config/application.local.json'), deps: [Config], multi: true }
        ]
} else {
    PROVIDERS = [
        { provide: 'settings.path', useValue: './assets/config/application.bb.json' },
        { provide: APP_INITIALIZER, useFactory: (config: Config) => () => config.load('/assets/config/application.bb.json'), deps: [Config], multi: true }
    ]
}

/**
 * `AppModule` is the main entry point into Angular2's bootstraping process
 */
@NgModule({
    bootstrap   : [AppComponent],
    declarations: [
        AppComponent,
        NoContentComponent,
        OffComponent,
        HeaderComponent,
        CarStatusComponent
    ],
    /**
     * Import Angular's modules.
     */
    imports     : [
        BrowserModule,
        FormsModule,
        HttpModule,
        LifeLineModule,
        NgxVirtualJoystickModule.forRoot(),
        ObserverModule,
        ObserverTabsModule,
        WebcamModule,
        RouterModule.forRoot(ROUTES, { useHash: true, preloadingStrategy: PreloadAllModules })
    ],
    /**
     * Expose our Services and Providers into Angular's dependency injection.
     */
    providers   : [
        ENV_PROVIDERS,
        APP_PROVIDERS,
        ...PROVIDERS,
        CarService,
        Config
    ]
})
export class AppModule {

    constructor(public appRef: ApplicationRef,
                public appState: AppState) {
    }

    public hmrOnInit(store: StoreType) {
        if (!store || !store.state) {
            return;
        }
        console.log('HMR store', JSON.stringify(store, null, 2));
        /**
         * Set state
         */
        this.appState._state = store.state;
        /**
         * Set input values
         */
        if ('restoreInputValues' in store) {
            let restoreInputValues = store.restoreInputValues;
            setTimeout(restoreInputValues);
        }

        this.appRef.tick();
        delete store.state;
        delete store.restoreInputValues;
    }

    public hmrOnDestroy(store: StoreType) {
        const cmpLocation = this.appRef.components.map((cmp) => cmp.location.nativeElement);
        /**
         * Save state
         */
        const state       = this.appState._state;
        store.state       = state;
        /**
         * Recreate root elements
         */
        store.disposeOldHosts = createNewHosts(cmpLocation);
        /**
         * Save input values
         */
        store.restoreInputValues = createInputTransfer();
        /**
         * Remove styles
         */
        removeNgStyles();
    }

    public hmrAfterDestroy(store: StoreType) {
        /**
         * Display new elements
         */
        store.disposeOldHosts();
        delete store.disposeOldHosts;
    }

}
