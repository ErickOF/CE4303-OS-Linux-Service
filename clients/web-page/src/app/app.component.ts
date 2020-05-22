import { Component } from '@angular/core';
import { ImageManipulationService } from './services/image-manipulation/image-manipulation.service';
import { Image } from './models/image';
import Swal from 'sweetalert2';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  private fileReader;
  // Image to process
  public img: Image
  public loading: boolean = false;
  // Image to show
  public source: string;
  // App title
  public title: string = 'Image Filter and Classifier';

  constructor(private imgManipulationService: ImageManipulationService) {
    this.fileReader = new FileReader();
    this.source = 'https://pngimage.net/wp-content/uploads/2018/06/lena-png-3.png'
  }

  public onChange(event: Event): void {
    let files = event.target['files'];
    this.loading = true;

    if (files) {
      this.fileReader.onload = () => {
        this.img = {
          name: 'Lena.png',
          source: this.fileReader.result.toString(),
          data: this.fileReader.result
        }
        this.loading = false;
      };
  
      this.fileReader.readAsDataURL(files[0]);
    }
  };

  public onProcessImage(): void {
    if (this.img) {
      const response = this.imgManipulationService.filterImage(this.img);
      response.subscribe((text) => {
        Swal.fire('Image was sent', 'Image was processing correctly.', 'info');
      }, (error) => {
        Swal.fire('Error!', 'Image wasn\'t sent', 'error');
      });
    } else {
      Swal.fire('Image does\'nt selected.', 'You must select an image.', 'warning');
    }
  }
}
