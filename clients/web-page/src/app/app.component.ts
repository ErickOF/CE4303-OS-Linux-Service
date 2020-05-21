import { Component } from '@angular/core';

declare interface Type {
  name: string;
}

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  public base64Files: string[] = [];
  private fileReader = new FileReader();
  public loading: boolean = false;
  public source: string = 'https://boofcv.org/images/thumb/6/66/Kodim17_noisy.jpg/300px-Kodim17_noisy.jpg'
  public title: string = 'Image Filter and Classifier';
  public type: Type;

  constructor() {
    this.type = {
      name: 'Filter'
    };
  }
  public onChange(event: Event) {
    let files = event.target['files'];
    this.loading = true;

    if (files) {
      this.fileReader.onload = () => {
        this.source = this.fileReader.result.toString();
        this.loading = false;
      };
  
      this.fileReader.readAsDataURL(files[0]);
      console.log(this.base64Files, this.fileReader)
    }
  };
}
