import { Component } from '@angular/core';
@Component({
    selector: 'observer-tabs',
    template: require('./observer-tabs.html'),
    styles: [require('./observer-tabs.scss')]
})
export class ObserverTabsComponent {
    private selectedTab = 'traffic_light';
}